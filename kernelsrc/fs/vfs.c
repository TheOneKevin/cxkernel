#include "fs/vfs.h"
#include "memory/kheap.h"
#include "system/kprintf.h"
#include "lib/tree.h"
#include "lib/exp_system.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

tree_t*   fs_tree = NULL;
vfsnode_t* fs_root = NULL; // MUST be a mountpoint

void vfs_init()
{
    fs_tree = tree_create();
    fs_root = (vfsnode_t *) kmalloc(kheap, sizeof(vfsnode_t));
    fs_root -> name = "/\0";
    fs_root -> path = "/\0";
    fs_tree -> root = tree_node_create(fs_root);
}

char *canonicalize_path(char *cd, char *input)
{
    return NULL;
}

/**
 * This will mount a node aMount onto a VFS path
 * @param  aMount The node describing the mountpoint_t
 * @param  path   The canonicalized path
 * @return        The status of the operation. 0 if successful
 */
status_t vfs_try_mount(fsnode_t* aMount, char* path)
{
    if (!fs_tree)
        return -1; //TODO: Errnos!
    if (!path || path[0] != PATH_SEPERATOR || !aMount)
        return -1;
    if (!aMount -> fs) //Sanity check to see if we have a filesystem
        return -1;

    char* p = strdup(path);
    int pathl = strlen(path);
    int i = pathl - 1;
    while (p[i] == PATH_SEPERATOR)
    {
        p[i] = 0; i--;
    }
    pathl = strlen(p);
    if (p[0] == 0 && p[1] == 0)
        kprintf("- Mounting: /\n");
    else
        kprintf("- Mounting: %s\n", p);
    i = 0;
    while (i < pathl)
    {
        if (p[i] == PATH_SEPERATOR)
            p[i] = 0;
        i++;
    }

    p[pathl] = 0;  //Make sure its null terminated

    if (p[1] == 0)  // In this case we are trying to mount the root fs
    {
        if (fs_root -> file)
            kprintf("    -> Path %s already mounted, unmount before trying to mount something else.\n", path);
        else
        {
            fs_root -> file = aMount;
            kprintf("    -> Root mounted.\n");
        }
    }
    else
    {
        char* token = p + 1; // Search through each path we tokenized
        tree_node_t* node = fs_tree -> root;
        while (token < p + pathl)
        {
            bool found = false;
            foreach(child, node -> children)
            {
                tree_node_t* tree_child = (tree_node_t *) child -> data;
                vfsnode_t* mount = (vfsnode_t *) tree_child -> data;
                if (strcmp(mount -> name, token) == 0)
                {
                    found = true;
                    node = tree_child;
                    mount -> refcount++;
                    break;
                }
            }

            if (!found)
            {
                // We did not find the directory, now making it.
                kprintf("    -> Node not found. Making: %s\n", token);
                vfsnode_t* mountp = (vfsnode_t *) kmalloc(kheap, sizeof(vfsnode_t));
                mountp -> name = strdup(token);
                mountp -> path = 0;
                mountp -> refcount = 1;
                tree_node_t* tmp = tree_node_create(mountp);
                tree_insert(fs_tree, node, tmp);
                node = tmp;
            }
            token = token + strlen(token) + 1;
        }

        vfsnode_t* mountp = (vfsnode_t *) node -> data;
        if (mountp -> file)
            kprintf("    -> Path %s already mounted, unmount before trying to mount something else.", path);
        else
        {
            mountp -> file = aMount;
            // Set the mount path, removing any trailing PATH_SEPERATORs
            char* tmp = strdup(path);
            i = strlen(tmp) - 1;
            while (tmp[i] == PATH_SEPERATOR)
            {
                tmp[i] = 0; i--;
            }
            mountp -> path = tmp;
        }
    }

    kfree(kheap, p);
    return 0;
}

status_t vfs_remove_mount(char* path)
{
    if (!fs_tree)
        return -1; //TODO: Errnos!
    if (!path || path[0] != PATH_SEPERATOR)
        return -1;

    char* p = strdup(path);
    int pathl = strlen(path);
    int i = pathl - 1;
    while (p[i] == PATH_SEPERATOR)
    {
        p[i] = 0; i--;
    }
    pathl = strlen(p);
    kprintf("- Unmounting: %s\n", p);

    while (i < pathl)
    {
        if (p[i] == PATH_SEPERATOR)
            p[i] = 0;
        i++;
    }

    p[pathl] = 0;  //Make sure its null terminated

    if (p[1] == 0)  // We are not allowed to unmount the root node.
    {
        kfree(kheap, p);
        return -1;
    }
    else
    {
        char* token = p + 1; // Search through each path we tokenized
        tree_node_t* node = fs_tree -> root;
        while (token < p + pathl)
        {
            bool found = false;
            foreach(child, node -> children)
            {
                tree_node_t* tree_child = (tree_node_t *) child -> data;
                vfsnode_t* mount = (vfsnode_t *) tree_child -> data;
                if (strcmp(mount -> name, token) == 0)
                {
                    found = true;
                    node = tree_child;
                    mount -> refcount--;
                    break;
                }
            }

            if (!found) // We did not find the directory so there is an error.
            {
                kfree(kheap, p);
                return -1;
            }

            token = token + strlen(token) + 1;
        }

        vfsnode_t* mountp = (vfsnode_t *) node -> data;
        kfree(kheap, mountp -> path);
        if (mountp -> file)
            kfree(kheap, mountp -> file);
        else
            kprintf("    -> There is no mount!\n");
    }

    kfree(kheap, p);
    return 0;
}

vfsnode_t* vfs_get_mount(char* path)
{
    char* p = strdup(path);
    int pathl = strlen(path);
    int i = 0;
    while (i < pathl)
    {
        if (p[i] == PATH_SEPERATOR)
            p[i] = 0;
        i++;
    }
    p[pathl] = 0;
    if (p[1] == 0)
    {
        kfree(kheap, p);
        return fs_root;
    }
    else
    {
        char* token = p + 1; // Search through each path we tokenized
        tree_node_t* node = fs_tree -> root;
        vfsnode_t* ret = fs_root;
        while (token < p + pathl)
        {
            foreach(child, node -> children)
            {
                tree_node_t* tree_child = (tree_node_t *) child -> data;
                vfsnode_t* mount = (vfsnode_t *) tree_child -> data;
                if (strcmp(mount -> name, token) == 0)
                {
                    node = tree_child;
                    if (mount -> path[0] != 0 /*&& mount -> file*/)  //Check if its a valid mount or is just a node
                        ret = mount;
                }
            }
            token = token + strlen(token) + 1;
        }
        kfree(kheap, p);
        return ret;
    }
    kfree(kheap, p);
    return fs_root;
}

uint32_t vfs_read(fsnode_t* file, void* buffer, uint32_t offset, uint32_t length)
{
    if (file -> file_ops -> read && fs_root -> file)
        return file -> file_ops -> read(file, buffer, offset, length);
    return 0;
}

uint32_t vfs_write(fsnode_t* file, void* buffer, uint32_t offset, uint32_t length)
{
    if (file -> file_ops -> write && fs_root -> file)
        return file -> file_ops -> write(file, buffer, offset, length);
    return 0;
}

fsnode_t* vfs_open(char* path)
{
    if (!path)
        return NULL;

    vfsnode_t* mountp = vfs_get_mount(path);

    if (!fs_root -> file || !mountp -> file)
        return NULL;
    if (!mountp -> file -> fs)
        return NULL;
    if (!mountp -> file -> fs -> fs_ops)
        return NULL;

    if (mountp -> file -> fs -> fs_ops -> open)
    {
        char* tmp = strdup(path);
        uint32_t foo = strlen(mountp -> path);
        tmp += foo == 1 ? foo - 1 : foo;
        int i = strlen(tmp) - 1;
        while (tmp[i] == PATH_SEPERATOR)
        {
            tmp[i] = 0; i--;
        }
        return mountp -> file -> fs -> fs_ops -> open(tmp, mountp -> file);
    }

    return NULL;
}

fsnode_t* vfs_touch(char* path)
{
    if (!path)
        return NULL;

    vfsnode_t* mountp = vfs_get_mount(path);

    if (!fs_root -> file || !mountp -> file)
        return NULL;
    if (!mountp -> file -> fs)
        return NULL;
    if (!mountp -> file -> fs -> fs_ops)
        return NULL;

    if (mountp -> file -> fs -> fs_ops -> touch)
    {
        char* tmp = strdup(path);
        uint32_t foo = strlen(mountp -> path);
        tmp += foo == 1 ? foo - 1 : foo;
        int i = strlen(tmp) - 1;
        while (tmp[i] == PATH_SEPERATOR)
        {
            tmp[i] = 0; i--;
        }

        return mountp -> file -> fs -> fs_ops -> touch(tmp, mountp -> file);
    }

    return NULL;
}

status_t vfs_delete(char* path)
{
    if (!path)
        return -1;

    vfsnode_t* mountp = vfs_get_mount(path);

    if (!fs_root -> file || !mountp -> file)
        return -1;
    if (!mountp -> file -> fs)
        return -1;
    if (!mountp -> file -> fs -> fs_ops)
        return -1;

    if (mountp -> file -> fs -> fs_ops -> remove)
    {
        char* tmp = strdup(path);
        uint32_t foo = strlen(mountp -> path);
        tmp += foo == 1 ? foo - 1 : foo;
        int i = strlen(tmp) - 1;
        while (tmp[i] == PATH_SEPERATOR)
        {
            tmp[i] = 0; i--;
        }
        return mountp -> file -> fs -> fs_ops -> remove(tmp, mountp -> file);
    }

    return -1;
}

void vfs_close(fsnode_t* file)
{
    if (!file || fs_root -> file)
        return;
    if (!file -> file_ops)
        return;
    if (file -> file_ops -> close)
        file -> file_ops -> close(file);
}

dirent_t* vfs_readdir(fsnode_t* file, uint32_t idx)
{
    if (!file || fs_root -> file)
        return NULL;
    if (!file -> file_ops)
        return NULL;
    if (file -> file_ops -> readdir)
        return file -> file_ops -> readdir(file, idx);
    return NULL;
}

fsnode_t* vfs_finddir(fsnode_t* file, char* name)
{
    if (!file || fs_root -> file)
        return NULL;
    if (!file -> file_ops)
        return NULL;
    if (file -> file_ops -> finddir)
        return file -> file_ops -> finddir(file, name);
    return NULL;
}

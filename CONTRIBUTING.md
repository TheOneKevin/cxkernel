Naming rules:

- Struct and class names must be in PascalCase
  - _Rule exceptions:_ Private/implementation-specific arch- or platform-specific structures
- All Struct/class members must be in lower_snake_case
  - Private members must additionally end in an underscore like_this\_
- Typedefs and usings must be in lower_snake_case

Code organization:

- Struct/class should have public members listed first and private members listed last
- Struct/class should be marked final whenever possible
- Using `noexcept` is optional as exceptions are disallowed anyways. `noexcept` may be required by the compiler or other compile-time checking tools, so they are not banned entirely.


Test naming convention:
- Unit tests are named like this: `test_domain unit_being_tested description...`
- For instance, "ebl intrusive_linked_list with refptr" tells us the domain is the "ebl" library and we're testing "intrusive_linked_list" specifically.

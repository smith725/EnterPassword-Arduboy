# EnterPassword-Arduboy
Simple one password manager for demonstration of basic concepts

This is an incredibly simple password manager, that manages only one password.

It is a testbed/proof of concept.

It lets you edit the password displayed on the screen. Basic edits are to change characters using up and down keys (faster if shifted with the B key). Movement is with left and right, while Bshift-left is delete left, and Bshift-right is insert a space.

In addition, there are several commands available.

1. Send - sends the current password out via USB keyboard functionality.
2. Random+Send - creates a new random password and immediately sends it via USB.
3. Random - creates a random password with a mix of letter, numbers, and symbols.
4. Random alphanumeric - creates a random password with a mix of letters and numbers.
5. Delete - clears the displayed password (not EEPROM).
6. Save - saves the currently displayed password to EEPROM.
7. Load - fetches a saved password froM EEPROM to the display.

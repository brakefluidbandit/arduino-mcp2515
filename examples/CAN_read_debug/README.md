# CAN_read_debug

There were several changes made:
* `mcp2515.h`
	* Added more error codes to replace the `ERROR_FAIL` error. By printing these errors, the exact function that failed can be found
* `mcp2515.cpp`
	* Any function that used to return `ERROR_FAIL` now returns a new error code that matches the function name. This makes it easier to find why something failed
	* `setMode()`: Changed the timeout from 10ms to 1000ms to give it a greater chance at succeeding. This is not a fantastic solution as it takes more time, but it does help with reliability and debugging
* `CAN_read_debug.ino`
	* a modified version of the original `CAN_read` sketch. This new version just has a lot of print statements to print out any time an error might happen.
	* `print_can()`: A new function that just prints the CAN message data stored in `canMsg`, which is a global varaible defined at the beginning of the sketch. This is just to make the `loop()` code easier to read and debug

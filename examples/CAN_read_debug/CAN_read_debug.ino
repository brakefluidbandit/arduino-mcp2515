/**
 * This version of the CAN_read sketch is identical to the original CAN_read sketch, but this version
 * has print statements to print out any possible error that might've occured when using an mcp2515
 * function that may throw an error.
 *
 * In addition, the CAN bitrate has been changed from 125KBPS to 500KBPS to match the F-CAN speed in most Honda vehicles
 * The MCP2515 crystal speed as well has been reduced to 8MHZ for better compatibility with other CAN bus shields
*/

#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg;
MCP2515 mcp2515(10);


void setup() {
  Serial.begin(115200);

  int err;
  err = mcp2515.reset();  // this will be non zero if it fails. can't say exactly what it'll be though
  Serial.println(err);
  err = mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);  // this will print 8 if this fails (ERROR_SET_BITRATE)
  Serial.println(err);
  err = mcp2515.setNormalMode();  // this will print 7 if this fails (ERROR_SET_MODE)
  Serial.println(err);

  if (err != MCP2515::ERROR_OK) {
    /**
     * If we get to this block of code, it means there was a problem during setup
     * we will just print the error code and loop forever because we don't want to continue with
     * an uninitialized
    */
    Serial.print("Setup error: ");
    Serial.println(err);
    while (err != MCP2515::ERROR_OK);
  }

  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");
}

/**
 * This code used to be in the loop() function, but it has been extracted out and moved to its own function for simplicity
 * This function takes the global canMsg can_frame struct and prints out the data contained within it.
 * This should only be called if canMsg is not NULL, otherwise bad things will probably happen.
*/
void print_can() {
  Serial.print(canMsg.can_id, HEX);      // print ID
  Serial.print(" ");
  Serial.print(canMsg.can_dlc, HEX);  // print DLC (data length code = number of bytes of data are in this CAN frame)
  Serial.print(" ");

  for (int i = 0; i < canMsg.can_dlc; i++) {  // print the data
    Serial.print(canMsg.data[i], HEX);
    Serial.print(" ");
  }

  Serial.println();
}

void loop() {
  int err = mcp2515.readMessage(&canMsg);
  if (err == MCP2515::ERROR_OK) {
    // If we get here, that means we successfully read from the CAN bus and saved the result. We can continue on to print
    print_can();
  } else {
    /**
     * If we get to this point, that means mcp2515.readMessage() returned some ERROR. We want to print out that
     * error and try reading the CAN bus again.
     *
     * There are many things that can cause an error, so this is helpful to see where the error comes from. The number that gets
     * printed out will match up with some error code (error codes are defined in mcp2515.h in the ERROR enum)
    */
    Serial.print("recv message error: ");
    Serial.println(err);
  }
}

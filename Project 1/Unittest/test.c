/*Unit Test to test various unit tests
 * Author: Harsimransingh Bindra & Yasir Aslam Shah
 * Advance Embedded System Design
 * Spring 2019
 * Definition: Test module to test various functionality for the unit conversion .Includes the following functions:
 * Functions: test_main()
 * */


#include "test_light.h"
#include "test_temp.h"
#include "test_conversion.h"

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  :Test main function , testing light, temp and conversion unit test
 *
 * @Returns   :returns EXIT status based on success or failure
 */
/* ----------------------------------------------------------------------------*/
int main(void)
{
  printf("**********TEST TEMPERATURE**********");
  main_temp();
  printf("**********TEST LIGHT**********");
  main_light();
  printf("**********TEST CONVERSION**********");
  main_conversion();
  printf("**********END**********");
  return EXIT_SUCCESS;
}

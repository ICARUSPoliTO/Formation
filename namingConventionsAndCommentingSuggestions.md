Naming Conventions.
--------

Variables use camelCase.
Example:

uint8_t angeloKnowsItAll = 1;

---

Library Instances use camelCase.
Example:

uint16_t irisSleepTime = iris_sleep_time();

---

Type definitions (like structs) use snake_case, unless CubeMX related (the latter use a different convention scheme, see the example in "Function parameters").
For further clarity, a "_t" might be added after the name to underline the presence of a specific variable type (depends on context and how long the name is).
Example:

typedef struct uncompensated_data {
    uint32_t temperature;
    uint32_t pressure;
} MS4525DO_uncompensated_data_t;

---

Constants use snake_case in capital letters.
Example:

#define TOOLBOX_ALWAYS_OPEN "YES"

---

Function names use snake_case, unless CubeMX related (the latter use a different convention scheme, see the example in "Function parameters").
Example:

lucaEatingSpeed = extract_last_digit_of_pi();

---

Function parameters use camelCase, but are often preceeded by the peripheral/sensor/library, separated by '_'.
Example:

void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);

----------------------------------------------------------------------------------


Comments suggestions.
--------

1) Be sure to indicate a unit of measurement whenever necessary, be it microseconds, kilometers, millibars or anything else
Examples:
//time for reading [us]
//lenght of travel [cm]

-------------------------------------------------------------------

2) To avoid problems when compiling and potentially improving readability, try to comment before the line you're mentioning and not next to it

Example of what to do:

//distance between point A and B [cm]
sensorReading = distance_from(pointA, pointB);

Example of what not to do:

sensorReading = distance_from(pointA, pointB); //distance between point A and B [cm]

Using the first method instead of the second also allows you to hover over any variable (on Visual Studio Code) and see its description (without changing the file/piece of code you're looking at)

-------------------------------------------------------------------

3) A certain specific syntax allows you to make a function easier to look at after use, allowing its description to be read from anywhere just by hovering your mouse over the function's name.
Example (the '*' are not necessary, they were inserted to underline that the lines are just placeholders):

/**
  * @brief *Short description, straight to the point.*
  *
  * @note *Longer description.*
  *
  * @param *varExample1: describes something in the function, and in here we have values from 5 to 25 [cm]*
  * @param *varExample2: describes something else in the function , and in here we have values from 0 to 100 [s]*
  *
  * @retval *Return value description*
  */
void function_example(uint8_t varExample1, uint8_t varExample2)
{
 *something happens in here*
}

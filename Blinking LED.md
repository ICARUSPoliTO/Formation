
1) Blinking LED: Toggle-Delay

In the main loop, insert the following:

HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); //Turn PIN from ON to OFF, or from OFF to ON (PINs' names and where they lead, along with what ports they have, can be found from documentation [*board model* datasheet, search it on Google])
HAL_Delay(1000); //wait 1000ms = 1s before restarting the loop



2) Blinking LED: Using a timer

From CubeMX:

On the left, select the timer section, then choose a general purpose timer (can be seen when hovering mouse on it).
Now on the timer, select the following settings:
-Clock source: Internal Timer
-TIMX Global Interrupt: Enabled [can be found under "NVIC Settings"]

--------------
Also, in the Parameter Settings, the Prescaler and Period need to be set, but what are they?

The prescaler is a number that we use to "divide" the clock into a smaller value, so that we can generate an interrupt on a certain specific time scale.
For example, if your clock is 16Mhz, generating an interrupt basing ourselves on a time scale of multiples of single clocks from the main timer of the processor would create an interrupt every N * 0.0000000625 seconds, which for blinking an LED on a human scale would be terribly fast and quite unmanageable. In order to get an on a more feasable time unit, we're going to divide the clock by a certain number.

A good value for this example (16Mhz) is a 16000 prescaler.
By division, we get 16MHz/16000 = 1000Hz, which means that we get a full timer counting cycle in (1/1000)s = 1ms.
Now we have set our timer to a unit on a reasonable scale for our interrupts, but how do we decide how often the interrupt actually happens?
That's the job of the second variable aforementioned: Period.

(N.B. The prescaler changes how the timer counts (without having any impact on the clock of the processor) and how fast a timer cycle happens, but on its own doesn't generate any interrupts) 
(N.B. To be more specific, the Prescaler should not be set to 16000, but to 16000 - 1, as what the value actually defines is the number of divisions, and not the pieces you're diving it into. Let's say you want to divide a cake in 3 pieces, how many times will you have to cut it? Yes, 3 - 1, exactly, that's what I mean) (it's not a big deal for big numbers, but it's good practise to keep it in mind)
(N.B. To see what the clock of your board is (and to possibly change it), go under the tab "Clock Configuration" on CubeMX, and in the schematics look for a slot called "HCLK (Mhz)"; after writing another number, remember to press enter to allow the board to "calculate" (solve the circuit) so that the new clock can actually be applied)

The period is a number that indicates how many times the timer cycle has to happen before we have an interrupt.
From the previous calculation, we discovered that (because of the Prescaler) a timer (not to be confused with the clock of the board) now cycles every millisecond, meaning that we can multiply 1ms with the number we decide as the period to have (finally!) our exact time for an interrupt.
In this case, we can select the Period to be 500, meaning that we're going to have an interrupt every 1ms * 500 = 500ms = 0.5s.
--------------

After the tiring explanation, here continuing with the LED blinking:

Start the timer adding, in main.c (inside //USER CODE BEGIN 2):

HAL_TIM_Base_Start_IT(&htimX); (substitute 'X' with the timer you're using)

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); (GPIOA is an example of a port, CubeMX will usually substitute it with oher easier to remember aliases) (same can be said for GPIO_PIN_5, example of a pin)
}



3) Blinking LED: Output Compare (Pure firmware: physically connecting your timer to the LED, can only be done if such connection is present for the PIN at hand)

Write, in main.c:

HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_1); (inside //USER CODE BEGIN 2)
In CubeMX, look for the PIN corresponding to your LED (for example, PA5, needs to be checked on the board's datasheet).
Configure the PIN to TIMX_CHY (X indicating the number of the timer, Y indicating the number of the channel: it will be different for every PIN, if available at all).
Set up TIMX with a clock source, Prescaler and Period just like in example number 2 (maybe you've already done it depending on which PIN you took there).
Enable CHY, going from "Disable" to "Output Compare CHY".
Go in the Parameter Settings, "Output Compare Channel Y", and put the mode to "Toggle on match".

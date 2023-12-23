# Lab 5, finally

To run this repo:

1. [Install Renode](https://renode.io/#downloads).
2. Clone this repo.
3. Run
   ```console
   $ make
   $ renode run.resc
   ```
   (requires GNU ARM toolchain in PATH).
4. Type `!RST#` into USART2 to start receiving value.
5. Type `!OK#` to stop.

Below is my explanation, and some problem when making STM32 work with Renode.

## Renode

Renode is basically a tool to simulate IoT systems. You will have:

- a hardware description file (see [stm32f4.repl](./stm32f4.repl))
- commands to simulate the system (see [run.resc](./run.resc))

Renode could be great in an educational context:

- Proteus is: 1) Windows only, 2) proprietary software. In contrast, Renode is
  cross-platform open source. This makes it suitable for everyone.
- Running CIs: You could automate grading with this. For example, labs can be
  made into Moodle quizzes.

## Platform description for STM32

I would recommend using STM32F4xx series. I could not get STM32F1xx to work,
unfortunately :(

The description at [stm32f4.repl](./stm32f4.repl) is actually modifies from the
main repo (see
https://github.com/renode/renode/blob/master/platforms/cpus/stm32f4.repl).

The following things are fixed:

### Timers

- For some reasons, Renode does not reload the correct value when counting
  down.This can be fix by setting the preload value directly in the description
  file.

- To modify the frequency, some frequency in the description file must be fiddle
  around, namely `nvic.systickFrequency` and `timer2.frequency`.

Here is an excerpt of the description file that makes it work in my case:

```
nvic: IRQControllers.NVIC @ sysbus 0xE000E000
    priorityMask: 0xF0
    systickFrequency: 168000000
    //                ^^^
    IRQ -> cpu@0

timer2: Timers.STM32_Timer @ sysbus <0x40000000, +0x400>
    -> nvic@28
    frequency: 1680000000
    //         ^^^
    initialLimit: 9999
    //            ^^^
```

To debug timers, the following command is useful: `machine GetClockSourceInfo`:

```
(machine-0) machine GetClockSourceInfo 
------------------------------------------------------------------------------------------------------------
|Owner                     |Enabled|Frequency|Limit               |Value |Step|Event frequency|Event period|
------------------------------------------------------------------------------------------------------------
|sysbus.adc1: samplingClock|True   |1MHz     |100                 |0     |1   |10kHz          |0.1ms       |
|sysbus.nvic: systick      |True   |0.17GHz  |167999              |134063|1   |1kHz           |1ms         |
|sysbus.iwdg: STM32_IWDG   |False  |8kHz     |4095                |4095  |1   |1.95Hz         |0.51s       |
|sysbus.rtc: ticker        |False  |0.26kHz  |256                 |256   |1   |1Hz            |1s          |
|sysbus.rtc: fastTicker    |False  |0.26kHz  |1                   |0     |1   |0.26kHz        |3.91ms      |
|sysbus.rtc: wakeupTimer   |False  |32.77kHz |18446744073709551615|0     |1   |0pHz           |562.95Ts    |
|sysbus.timer2             |False  |1MHz     |9999                |9999  |1   |0.1kHz         |10ms        |
|sysbus.timer2: cctimer1   |False  |1MHz     |9999                |0     |1   |0.1kHz         |10ms        |
|sysbus.timer2: cctimer2   |False  |1MHz     |9999                |0     |1   |0.1kHz         |10ms        |
|sysbus.timer2: cctimer3   |False  |1MHz     |9999                |0     |1   |0.1kHz         |10ms        |
|sysbus.timer2: cctimer4   |False  |1MHz     |9999                |0     |1   |0.1kHz         |10ms        |
------------------------------------------------------------------------------------------------------------
```

I fiddled with the parameters until the `sysbus.timer2`'s Event period yielded
10ms.

### ADC

For ADC, add the followings works:

```
adc1: Analog.STM32_ADC @ sysbus 0x40012000
    IRQ -> nvic@18
```

To add value to ADC: `sysbus.adc1 FeedSample <value> 0 <repeat>`. Currently, the
sample speed is at 1kHz and cannot be change, AFAIK.

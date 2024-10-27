# 20-20-20

Arduino reminder for when you sit too long.

Required:

- US sensor or tilt sensor
- passive buzzer

Functionality:
Buzzes alarmingly when your 20 minutes is up, counts for 20 seconds audibly.
Windows-like sounds for when you open or close the laptop.

You can change:

- Buzzer pitch and duration in buzzer functions - tone(pin, frequency, duration in ms)
- sitTimeLimit in seconds
- breakDuration in seconds
  Ultrasonic sensor version:
- Might require re-calibration with your US sensor but generally should work.
- distanceThreshold is how far from behind the open laptop screen the sensor is. For it to identify an open screen it must detect it within that distance in cm.

distance was unreliable and the sensor makes a clicky noise and is easy to knock over, so I modified it into a tilt sensor version.

Tilt sensor version:
The tilt sensor is stuck to the back of the screen such that it outputs LOW (disconnected) when the screen is upright.

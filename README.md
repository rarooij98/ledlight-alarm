# Ledlight alarm clock :alarm_clock:
How to turn on a ledstrip based on time (or using a public transport api)?

My challenge was to turn on a ledstrip at a certain time based on data from a Dutch public transport api: NS Reisinformatie API. <br>
The first step is to make the ledstrip turn on at a certain time, for example 09.00 AM. :clock9: <br>
After that I want the ledstrip to turn on 10 minutes earlier when there is a calamity, disruption or maintenance in public transport. :construction:

:calendar: **Date:** 26/10/2022

### What do you need for this project?
- NodeMCU esp8266 board
- RGB LED strip
- Install: Arduino IDE, Telegram app, Adafruit IO account
- Wifi (not 5G)

## 1. Turning on the ledstrip. :bulb:
The first step is to make the ledstrip turn on at a certain time. How can my ESP know the time?

<img src="https://github.com/rarooij98/telegram-on-esp8266/blob/main/images/" width=25% height=25%>

## 2. Controlling the ledstrip on the web. :computer:
Now we can set the ledstrip to a certain time, it would be great if we could also set and change the turn-on time in a web app. 

Adafruit IO / Telegram

To interact with the Telegram bot I needed to install a library called Universal Telegram Bot Library by Brian Lough. 
I searched for it in the Arduino Library Manager and installed the lastest version. You can also download the zip file from https://www.arduinolibraries.info/libraries/universal-telegram-bot and include it in your project in the Sketch tab.

<img src="https://github.com/rarooij98/telegram-on-esp8266/blob/main/images/" width=20% height=20%>

### Error :triangular_flag_on_post:
I couldn't find ...

<img src="https://github.com/rarooij98/telegram-on-esp8266/blob/main/images/" width=25% height=25%>

After installing the library, I copied this code into my sketch:

<img src="https://github.com/rarooij98/telegram-on-esp8266/blob/main/images/" width=30% height=30%>

To make this code work for me I had to insert my Bot token, user ID, and network credentials (SSID and password). 

### What does this code do?
This code checks for new messages every second, it then also checks the chat_id to see if the message is from you or if it should be ignored. If it's from you, it saves the message in a text variable and checks its content, and when it receives the **/...** message it ... and sends a confirmation message.

## 3. NS Reisinformatie API. :bullettrain_side:
We can now set a time for out ledstrip to turn on, just like a real alarm clock. :tada:
But if we want to make this even more interesting, we should use data from the NS Reisinformatie API.
To use this API you need to create a free account at https://apiportal.ns.nl/signin and subscribe to their Reisinformatie API. After diong this you can find your api key on your profile page.

<img src="https://github.com/rarooij98/telegram-on-esp8266/blob/main/images/" width=30% height=30%>

The NS API website has a lot of great code examples, but unfortunately none of them are for Arduino/C++.

<img src="https://github.com/rarooij98/telegram-on-esp8266/blob/main/images/" width=30% height=30%>

So I had to look for other sources on how to connect and get data from this api, and I used this manual that explains how to get weather data: https://www.dfrobot.com/blog-917.html.

I also watched this video on how to connect to an API using an ESP8266 (or any arduino): https://www.youtube.com/watch?v=HUjFMVOpXBM. This one was very helpful but only covered part of what I needed to do. 




### Error :triangular_flag_on_post:

When following the examples listed above, I kept getting this errors about the HttpCLient librarcy:

<img src="https://github.com/rarooij98/telegram-on-esp8266/blob/main/images/" width=30% height=30%>


#### :rotating_light: Problem 1


#### :rotating_light: Problem 2 


## Sources :card_file_box:
- https://www.ns.nl/reisinformatie/ns-api
- https://www.dfrobot.com/blog-917.html
- https://www.youtube.com/watch?v=HUjFMVOpXBM

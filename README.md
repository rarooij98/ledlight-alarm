# Ledlight alarm clock :alarm_clock:
How to turn on a ledstrip based on time (or using a public transport api)?

My challenge was to turn on a ledstrip at a certain time based on data from a Dutch public transport api: NS Reisinformatie API. <br>
The first step is to make the ledstrip turn on at a certain time, for example 09.00 AM. :clock9: <br>
After that I want the ledstrip to turn on 10 minutes earlier when there is a calamity, disruption or maintenance in public transport. :construction:

:calendar: **Date:** 26/10/2022

### What do you need for this project?
- NodeMCU esp8266 board
- RGB LED strip
- Arduino IDE & Wifi (not 5G)

## 1. Turning on the ledstrip :bulb:
I changed my pin to D5 and my number of pixels to 14. I you haven't connected you ledstrip yet, this is how I did it:
The 5V wire goes to 3V3, the GND wire goed to GND, and the middle wire (Din) goes to D5.

<img src="https://github.com/rarooij98/ledlight-alarm/blob/main/images/wires.PNG" width=40% height=40%> 

<img src="https://github.com/rarooij98/ledlight-alarm/blob/main/images/IMG_20221026_122143.jpg" width=20% height=20%>

First I uploaded the example code Simple from Adafruit Neopixel to my board to test whether my ledstrip turns on.

<img src="https://github.com/rarooij98/ledlight-alarm/blob/main/images/example2.PNG" width=60% height=60%> <img src="https://github.com/rarooij98/ledlight-alarm/blob/main/images/IMG_20221026_122045.jpg" width=30% height=30%>

Succes! :tada:

## 2. Setting a turn-on time :clock9:
The next step is to make the ledstrip turn on at a certain time.How can I make my ESP know the time? To do this I used the Time library by Michael Margolis. I read the documentation for this library here: https://playground.arduino.cc/Code/Time/

I needed to set my timezone & connect to the internet to get the time from the Network Time Protocol (NTP). You can find your timezone string on this site: https://remotemonitoringsystems.ca/time-zone-abbreviations.php

<img src="https://github.com/rarooij98/ledlight-alarm/blob/main/images/timezone.PNG" width=60% height=60%>

To test my code I added this function to print the day of the week and current time:

```
void showTime(tm localTime) {
  Serial.print(localTime.tm_mday);
  Serial.print('/');
  Serial.print(localTime.tm_mon + 1);
  Serial.print('/');
  Serial.print(localTime.tm_year - 100);
  Serial.print('-');
  Serial.print(localTime.tm_hour);
  Serial.print(':');
  Serial.print(localTime.tm_min);
  Serial.print(':');
  Serial.print(localTime.tm_sec);
  Serial.print(" Day of Week ");
  if (localTime.tm_wday == 0)   Serial.println(7);
  else Serial.println(localTime.tm_wday);
}
```

It printed this info to the Serial Monitor every second:

<img src="https://github.com/rarooij98/ledlight-alarm/blob/main/images/printtime.PNG" width=40% height=40%>

Great! :tada:

Now I have to set this data as conditions for the ledstrip to turn on. I did this by writing an if-statement.

If I want to turn on the lights at 09.00 AM, I should write in the if-statament: 

```
if (localTime.tm_hour == 9) {...}
```

When I run this code, the lights will go on at the set time and also print out the current time:

```
    if (localTime.tm_hour == 9) {
      Serial.print("Time to turn on the lights at: ")
      Serial.print(localTime.tm_hour);
      Serial.print(':');
      Serial.print(localTime.tm_min);

      for(int i=0; i<NUM_PIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(255, 255, 255));
        pixels.show();
        delay(1000);
      }
    }
```
To test the code I put in the current hour and minute like this:

```
if (localTime.tm_hour == 14 && localTime.tm_min == 32)
```

Do the lights go on at the set time?

<img src="https://github.com/rarooij98/ledlight-alarm/blob/main/images/ontime.PNG" width=60% height=60%>
<img src="https://github.com/rarooij98/ledlight-alarm/blob/main/images/IMG_20221026_143456.jpg" width=30% height=30%>

Yes they do! :tada:

## 3. NS Reisinformatie API. :bullettrain_side:
We can now set a time for our ledstrip to turn on, just like a real alarm clock. :tada:
But if we want to make this even more interesting, we should use data from the NS Reisinformatie API.

### Create your NS API account
To use this API you need to create a free account at https://apiportal.ns.nl/signin and subscribe to their Reisinformatie API. After doing this you can find your api key on your profile page:

<img src="https://github.com/rarooij98/ledlight-alarm/blob/main/images/api.PNG" width=40% height=40%> <img src="https://github.com/rarooij98/ledlight-alarm/blob/main/images/apikey.PNG" width=50% height=50%>

The NS API website has a lot of great code examples, but unfortunately none of them are for Arduino/C++.

<img src="https://github.com/rarooij98/ledlight-alarm/blob/main/images/codexamples.PNG" width=50% height=50%>

So I had to look for other sources on how to connect and get data from this api, and I used this manual that explains how to get weather data: https://www.dfrobot.com/blog-917.html. I also watched this video on how to connect to an API using an ESP8266 (or any arduino): https://www.youtube.com/watch?v=HUjFMVOpXBM. This one was very helpful but only covered part of what I needed to do.

### Connection string & API key
On the NS API website you can find the connection string for the API you want to connect to. I need to put this string and our api key in our code:

```
const String endpoint = "https://gateway.apiportal.ns.nl/reisinformatie-api/api/v3/disruptions[?type][&isActive]";
const String key = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
```

I will use these endpoint and key variables later in void loop().

### GET request
Next I needed to do a GET request in the loop. This will get me data about the delays/calamities.
This is the code I used with the HttpClient library. 

```
void loop() {
 
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
    
    HttpClient http;
 
    http.begin(endpoint + key); //Specify the URL
    int httpCode = http.GET();  //Make the request
 
    if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
  }
```

### Error :triangular_flag_on_post:

I kept getting this error about HttpClient:
#### :rotating_light: No matching function for call to 'HttpClient::HttpClient()'

<img src="https://github.com/rarooij98/ledlight-alarm/blob/main/images/errorhttp.PNG" width=80% height=80%>

I found an answer on the Arduino forum: https://forum.arduino.cc/t/no-matching-function-to-call-for-httpclient/688817

<img src="https://github.com/rarooij98/ledlight-alarm/blob/main/images/answer.PNG" width=80% height=80%>

This linked me to the libraries Github page where I could find more information. The error was partly fixed by including the EthernetClient library and initializing the http client this way:

```
WiFiClient c;
HttpClient http(c);
```

### Error :triangular_flag_on_post:
But I still had another error with this library: 
#### :rotating_light: 'class HttpClient' has no member named 'begin'
<!--#### :rotating_light: call to 'HTTPClient::begin' declared with attribute error: obsolete API, use ::begin(WiFiClient, url)-->

First I tried deleting and redownloading the library but that didn't do anything.
I couldn't figure it out so I decided to use the method of this source: https://randomnerdtutorials.com/esp8266-nodemcu-http-get-post-arduino/ and use these libraries instead:

```
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Arduino_JSON.h>
```

### Error :triangular_flag_on_post:
After that I didn't get any error messages anymore but I also couldn't request anything.  

I tried to give the URL & API key in the following line:

```
http.begin(client, endpoint + key);
```

But something must be wrong, because I cannot request anything and get the 'Error on HTTP request' message in the Serial Monitor.

<img src="https://github.com/rarooij98/ledlight-alarm/blob/main/images/httpbegin.PNG" width=50% height=50%> <img src="https://github.com/rarooij98/ledlight-alarm/blob/main/images/reqerror.PNG" width=40% height=40%>

## Sources :card_file_box:
- https://playground.arduino.cc/Code/Time/
- https://remotemonitoringsystems.ca/time-zone-abbreviations.php
- https://www.ns.nl/reisinformatie/ns-api
- https://www.dfrobot.com/blog-917.html
- https://www.youtube.com/watch?v=HUjFMVOpXBM
- https://randomnerdtutorials.com/esp8266-nodemcu-http-get-post-arduino/

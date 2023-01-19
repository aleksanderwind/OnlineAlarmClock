#ifndef html_page_h
#define html_page_h
/*
* Define html page and store it in flash memory.
* Authors: AC, JKT
*/
const char index_html[] PROGMEM = R"rawliteral(
<html>

<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <title>Online Alarm Clock</title>
    <meta name="final project" content="">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <h1> Online Alarm Clock</h1>
</head>
   

<style> 
    input.mode1[type="submit"]{
        float: left;
        height: 25px;
        width: fit-content;
        background-color: #ff0000;
        color: white;
        text-align: center;
        margin-right: 10px;
    }

    input.mode2[type="submit"]{
        float: left;
        height: 25px;
        width: fit-content;
        background-color: #0000ff;
        color: white;
        text-align: center;
        margin-right: 10px;
    }

    input.off[type="submit"]{
        float: left;
        height: 25px;
        width: fit-content;
        background-color: #000000;
        color: white;
        text-align: center;
    }

    input.set[type="submit"]{
        height: 25px;
        width: fit-content;
        background-color: #000000;
        color: white;
        text-align: center;
        margin-top: 4px;
        margin-left: 10px;
    }

    input[type="color"] {
        opacity: 0;
        display: block;
        width: 32px;
        height: 32px;
    }
    #color-picker-wrapper {
        float: left;
        border: 10px;
        border-radius: 50%;
        margin-right: 10px;
        border: 1px solid black;
    }

    input[type="time"]{
        height: 21px;
        width: fit-content;
        background-color: aqua;
        margin-left: 10px;
    }

    input[type="date"]{
        height: 21px;
        width: fit-content;
        background-color: aqua;
        margin-left: 11.5px;
        margin-bottom: 2px;
    }

    input[type="number"]{
        height: 21px;
        width: 50px;
        margin-left: 4px;
        margin-right: 2px;
    }

    p.inline {
        display: inline;
    }

    b.big {
        font-size: 150%;
        color: rgb(0, 0, 0);
    }

    hr.splitLine {
        float: left;
        height: 2px;
        width: 400px;
        border-width: 0;
        color: #000000;
        background-color: #000000;
    }

</style>

<body>
    <hr class="splitLine"><br>
    <b class="big"> LED control</b>
    <div>
        <form action="/mode1">
            <input class = mode1 type="submit" value = "Mode 1">
        </form>
    
        <form action="/mode2">
            <input class = mode2 type="submit" value = "Mode 2">
        </form>
    
        <form action="/off">
            <input class = off type="submit" value = "Off"><br><br>
        </form>
    </div>
    
    <label for="staticColor">Select color of LED strip:</label>
    <form action="/setStaticColor">
        <div id="color-picker-wrapper">
            <input type="color" value="#ff0000" id="staticColor" name="staticColor">
        </div>
        <input class = set type="submit" value = "Set color"> <br><br>
    </form>

    <hr class="splitLine"><br>
    <b class="big"> Alarm control</b>

    <div>
        <form action = "/setAlarm">
            <label for="alarmDate"> Select date for alarm: </label>
            <input type="date" id="alarmDate" name="alarmDate"><br>
            <label for="alarmTime"> Select time for alarm: </label>
            <input type="time" id="alarmTime" name="alarmTime"><br>
            <label for="timeInterval">Select fade in interval: </label>
            <input type="number" id="timeInterval" name="timeInterval">
            <input class = set type="submit" value = "Set alarm"><br>
        </form>
    </div>

    <p class = "inline">Date of alarm: </p>
    <p class = "inline" style="background-color: aqua;" id = "dateOfAlarm"></p><br>
    <p class = "inline">Time of alarm: </p>
    <p class = "inline" style="background-color: aqua;" id = "timeOfAlarm"></p>


    <form action="/setWakeUpSong">
        <p>Choose wakeup song: </p>
        <input type="radio" id="song1" name="songID" value="1">
        <label for="song1">Pirates of the Caribbean</label>
        <input type="radio" id="song2" name="songID" value="2">
        <label for="song2">Crazy Frog</label> <br>
        <input type="radio" id="song3" name="songID" value="3">
        <label for="song3">Mario</label>
        <input style="margin-left: 120px;" type="radio" id="song4" name="songID" value="4">
        <label for="song4">Pink Panther</label>
        <input style="margin-left: 10px;" class="set" type="submit" value="Set song"><br><br>
    </form>

    <hr class="splitLine"><br>
    <b class="big"> Data center</b><br>

    <p class="inline">Current day and time: </p>
    <p class = "inline" id = "currentDay"></p>
    <p class = "inline">     </p>
    <p class = "inline" id = "currentHour"></p>
    <p class = "inline">:</p>
    <p class = "inline" id = "currentMinute"></p><br><br>

    <p class ="inline" id="temperatureText"> Temperature: </p>
    <p class = "inline" id="temperature"></p>
    <p class ="inline"> &#x2103;</p><br>
    <p class ="inline" id="humidText"> Humidity: </p>
    <p class = "inline" id="humidity"></p>
    <p class ="inline"> %</p><br>
    <p class ="inline"> Light level: </p>
    <p class = "inline" id="ligthLevel"></p>
    <p class ="inline"> lm</p><br>

    <hr class="splitLine">

    <script>
        window.addEventListener('load', getCurrentColor);

        function getCurrentColor(){
          var xhr = new XMLHttpRequest();
          xhr.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
              var val = this.responseText;
              console.log(val);
              document.getElementById('staticColor').value = val;
              document.getElementById('color-picker-wrapper').style.backgroundColor = val;
            }
          }; 
          xhr.open("GET", "/getColor", true);
          xhr.send();
        }

        var color_picker = document.getElementById("staticColor");
        var color_picker_wrapper = document.getElementById("color-picker-wrapper");
        color_picker.onchange = function () {
            color_picker_wrapper.style.backgroundColor = color_picker.value;
        }
        color_picker_wrapper.style.backgroundColor = color_picker.value;


        window.addEventListener('load', getCurrentAlarm);

        function getCurrentAlarm(){
            var xhr = new XMLHttpRequest();
            xhr.onreadystatechange = function(){
                if (this.readyState == 4 && this.status == 200) {
                var val = this.responseText;
                console.log(val)
                var dateAndTime = val.split("#");
                document.getElementById('alarmDate').value = dateAndTime[0];
                document.getElementById('alarmTime').value = dateAndTime[1];
                document.getElementById('dateOfAlarm').innerHTML = dateAndTime[0];
                document.getElementById('timeOfAlarm').innerHTML = dateAndTime[1];
                document.getElementById('timeInterval').value = dateAndTime[2];
                }
            };
            xhr.open("GET", "/getAlarmDateAndTime", true);
            xhr.send();
        }

        window.addEventListener('load', getCurrentWakeUpSong);

        function getCurrentWakeUpSong(){
            var xhr = new XMLHttpRequest();
            xhr.onreadystatechange = function(){
                if (this.readyState == 4 && this.status == 200) {
                var val = this.responseText;
                console.log(val)
                if(val == "1"){
                    document.getElementById("song1").checked = true;
                }
                if(val == "2"){
                    document.getElementById("song2").checked = true;
                }
                if(val == "3"){
                    document.getElementById("song3").checked = true;
                }
                if(val == "4"){
                    document.getElementById("song4").checked = true;
                }
                }
            };
            xhr.open("GET", "/getCurrentWakeUpSong", true);
            xhr.send();
        }

        const someInterval = setInterval(updatePage, 4000);

        function updatePage(){
            var xhr = new XMLHttpRequest();
            xhr.onreadystatechange = function(){
                if (this.readyState == 4 && this.status == 200) {
                var val = this.responseText;
                console.log(val)
                var dayHourMinuteTempHumLum = val.split("#");
                document.getElementById("currentDay").innerHTML = dayHourMinuteTempHumLum[0];
                document.getElementById("currentHour").innerHTML = dayHourMinuteTempHumLum[1];
                document.getElementById("currentMinute").innerHTML = dayHourMinuteTempHumLum[2];
                document.getElementById("temperature").innerHTML = dayHourMinuteTempHumLum[3];
                document.getElementById("humidity").innerHTML = dayHourMinuteTempHumLum[4];
                document.getElementById("ligthLevel").innerHTML = dayHourMinuteTempHumLum[5];

                if (parseFloat(dayHourMinuteTempHumLum[3]) >= 30.0){
                    document.getElementById("temperatureText").style.backgroundColor = "#ff0000";
                }else if (parseFloat(dayHourMinuteTempHumLum[3]) <= 10.0){
                    document.getElementById("temperatureText").style.backgroundColor = "#0000ff";
                }else{
                    document.getElementById("temperatureText").style.backgroundColor = "#00ff00";
                }

                if (parseFloat(dayHourMinuteTempHumLum[4]) >= 60.0){
                    document.getElementById("humidText").style.backgroundColor = "#ff0000";
                }else if (parseFloat(dayHourMinuteTempHumLum[4]) <= 40.0){
                    document.getElementById("humidText").style.backgroundColor = "#ff0000";
                }else{
                    document.getElementById("humidText").style.backgroundColor = "#00ff00";
                }
                
                }
            };
            xhr.open("GET", "/updatePage", true);
            xhr.send();
        }


    </script>
    
</body>

</html>
)rawliteral";
#endif //html_page_h

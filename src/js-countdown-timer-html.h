const char clockhtml[] PROGMEM = {R"=====(
<!DOCTYPE html>
<html>
  <head>
    <title>
      Hose Timer
    </title>
<!--  

      CSS section
      
-->
    <style>
.probes-container {
  height:100vh;
  overflow-y: auto;
}
.boxes {
  width: 90vw;
  height: auto;
  margin-bottom: 1vh;
  padding: 2vh 0;
  background: #404040;
  border-radius: 0.5vh;
}

.button {
  border-radius: 1vh;
}
/* (A) FONT */
#ctime, #tpick {
  font-family: sans-serif;
}
.header {
  text-align: center;
  font-weight: normal;
  margin: 5px 0 10px 0;
}

/* (B) CURRENT TIME */
#ctime {
  width: 90vw;
  height: auto;
  margin-bottom: 1vh;
  padding: 2vh 0;
  background: #404040;
  border-radius: 1vh;
  background: rgb(255, 0, 0);
  text-align: center;
}
#ctime .header {
  color: #c61d1d;
}
#ctime .square {
  display: inline-block;
  padding: 0px;
  margin: 0px;
}
#ctime .digits {
  font-size: 15vh;
  font-weight: bold;
  background: rgba(255, 255, 255, 0);
  color: #fff;
  border-radius: 5px;
}
#ctime .text {
  margin-top: 15vh;
  color: #ddd;
}

/* (C) TIME PICKER */
#tpick {
  width: 90vw;
  height: auto;
  margin-bottom: 1vh;
  padding: 2vh 0;
  border-radius: 1vh;
  background: #f2f2f2;
  white-space: nowrap;
}
#tpick-m {
  width: 100%;
}
#tpick select {
  width: 85vw;
  font-size: 15vh;
  text-align-last: center;
  font-weight: bold;
  margin: 20px 0;
  border-radius: 1vh;
}

#tstart {
  width: 85vw;
  height: auto;
  font-size: 15vh;
  font-weight: bold;
  margin-bottom: 1vh;
  padding: 2vh 0;
  background: #0c9113;
  color: #fff;
  border: 0;
  cursor: pointer;
}
#tstop {
  width: 85vw;
  height: auto;
  font-size: 15vh;
  font-weight: bold;
  margin-bottom: 1vh;
  padding: 2vh 0;
  background: #ff0000;
  color: #fff;
  border: 0;
  cursor: pointer;
}
#tstart:disabled, #tstop:disabled {
  background: #aaa;
  color: #888;
}
    </style>

<!-- 

      javascript section
      
-->
    <script>
  var ac = {

  // (A) INITIALIZE TIMER
  init : function () {
    // (A1) CREATE REFERENCES FOR CLOCK, MIN, SECONDS
    ac.ctime = document.getElementById("ctime");
    ac.cmin = document.getElementById("cmin");
    ac.csec = document.getElementById("csec");

    // (A2) CREATE TIME PICKER - MIN
    ac.thm = ac.createSel(10);
    document.getElementById("tpick-m").appendChild(ac.thm);
    ac.thm.addEventListener("change", ac.thmChange);
    ac.thmChange();

    // (A3) CREATE TIME PICKER - startTimer, stopTimer
    ac.tstart = document.getElementById("tstart");
    ac.tstart.addEventListener("click", ac.startTimer);
    ac.tstop = document.getElementById("tstop");
    ac.tstop.addEventListener("click", ac.stopTimer);

    // (A4) GET ALARM SOUND
    ac.sound = document.getElementById("alarm-sound");
    ac.sound.loop = false;
    
    // (A5) START THE CLOCK
    var now = new Date().getTime();
    ac.tRemain = new Date().setTime(0);
    ac.alarm = null;

    // FETCH the remaining time from the server in case the timer was started by someone else
    ac.GETupdate();
    ac.tickInterval = setInterval(ac.tick, 1000);
  },

  // (B) SUPPORT FUNCTION - CREATE SELECTOR FOR HR, MIN, SEC
  createSel : function (max) {
    const choices = [10,20,30,0.1];
    var selector = document.createElement("select");
    for (let i in choices) {
      var opt = document.createElement("option");
      opt.style.direction = "rtl";
      opt.style.fontWeight = "bold";
      temp = choices[i];
      opt.value = temp;
      opt.innerHTML = temp;
      selector.appendChild(opt);
    }
    return selector
  },

  // (C) SUPPORT FUNCTION - PREPEND HR, MIN, SEC WITH 0 (IF < 10)
  padzero : function (num) {
    if (num < 10) { num = "0" + num; }
    else { num = num.toString(); }
    return num;
  },

  // (D) UPDATE CURRENT TIME
  tick : function () {
    //console.log("TICK");
    // get the time from the server
    ac.GETupdate();
    ac.updateClock(ac.tRemain);

    if (ac.timerState) 
    {
      // (D3) CHECK AND SOUND ALARM
        if (ac.tRemain <= 0) {
          if (ac.sound.paused) { ac.sound.play();
          ac.stopTimer();
          }
        }
    }

  },

  updateClock : function (_tRemain) {
    if(_tRemain>0)
    {
    // Time calculations for minutes and seconds
    var minutes = Math.floor((_tRemain % (1000 * 60 * 60)) / (1000 * 60));
    var seconds = Math.floor((_tRemain % (1000 * 60)) / 1000);
    var min = ac.padzero(minutes);
    var sec = ac.padzero(seconds);

    // (D2) UPDATE HTML CLOCK
    ac.cmin.innerHTML = min;
    ac.csec.innerHTML = sec;
    //ac.tRemain = _tRemain;
    //console.info("updateClock: ac.tRemain = " + ac.tRemain);
    }
  },

  // (E) startTimer 
  startTimer : function () {
    if (!ac.timerState) 
    {
      // send POST with ac.thm.value (duration in milliseconds) so the server knows to start the device
      ac.SETtRemain(ac.thm.value*60000);
      ac.sound.pause();
    }
    ac.thm.disabled = true;
    ac.tstart.disabled = true;
    ac.tstop.disabled = false;
    // green background because we should be counting down
    ac.ctime.style.backgroundColor = "green";
    ac.timerState = true;
  },

  // (F) stopTimer 
  stopTimer : function () {
    //if (!ac.sound.paused) { ac.sound.pause(); }
    ac.thm.disabled = false;
    ac.tstart.disabled = false;
    ac.tstop.disabled = true;
    ac.ctime.style.backgroundColor = "red";
    ac.updateClock(ac.thm.value*60000);
    // send POST with zero "0" to turn off device
    ac.SETtRemain(0);
    ac.timerState = false;
  },

  SETtRemain : async function (data) {
    try {
      // disable tick during POST
      clearInterval(ac.tickInterval);
      console.log("tick OFF");
      ac.tRemain = parseInt(data);
      console.log("SETtRemain: ac.tRemain = "+ac.tRemain.toString());

      const response = await fetch('/tRemain',{ 
        method: 'POST',
        mode: 'cors', // no-cors, *cors, same-origin
        cache: 'no-cache', // *default, no-cache, reload, force-cache, only-if-cached
        credentials: 'omit', // include, *same-origin, omit
        headers: {
          'Content-Type': 'application/json'
        },
        redirect: 'follow', // manual, *follow, error
        referrerPolicy: 'no-referrer', // no-referrer, *no-referrer-when-downgrade, origin, origin-when-cross-origin, same-origin, strict-origin, strict-origin-when-cross-origin, unsafe-url
        body: JSON.stringify(data) // body data type must match "Content-Type" header
        });

      const tempval = await response.json();
    }
    catch (error) {
      console.error(error);
    }
    // re-enable tick after POST
    ac.tickInterval = setInterval(ac.tick, 1000);
    console.log("tick ON");

  },

  GETupdate : function() {
    fetch('http://hosetimer/tRemain', { method: 'GET' })
      .then(response => response.text()) 
      .then(function(mydata) {
        ac.tRemain = parseInt(mydata);
        if(ac.timerState) console.info("fetch mydata = " + mydata);
        if(ac.tRemain>0 && !ac.timerState)
        {
          ac.timerState = true;
          ac.startTimer();
        }
      })
      .catch(err => console.log(err));
  },

  thmChange : function () {
    ac.updateClock(ac.thm.value*60000);
  }
};

window.addEventListener("load", ac.init);
  </script>
  </head>
  <body>
  <center>
    <div class='main-container' id='mainIndex'>
      <!-- (A) CURRENT TIME -->
      <div id="ctime">
        <div class="square">
          <div class="digits" id="cmin">0</div>
        </div>
        <div class="square">
          <div class="digits">:</div>
        </div>
        <div class="square">
          <div class="digits" id="csec">00</div>
        </div>
      </div>

      <!-- (B) startTimer -->
      <div id="tpick">
        <div id="tpick-m"></div>
        <div>
          <input type="button" class="button" value="Start" id="tstart"/>
        </div>
        <div>
          <input type="button" class="button" value="Stop" id="tstop" disabled/>
        </div>
      </div>
    </div>
  </center>
    <!-- (C) ALARM SOUND -->
    <audio id="alarm-sound">
      <source src="https://upload.wikimedia.org/wikipedia/commons/d/d9/Wilhelm_Scream.ogg" type="audio/ogg">
    </audio>
  </body>
</html>
)====="};
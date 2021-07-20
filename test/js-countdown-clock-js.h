const char clockjs[] PROGMEM = {R"=====(
  var ac = {
  // (A) INITIALIZE ALARM CLOCK
  init : function () {
    // (A1) GET THE CURRENT TIME - HOUR, MIN, SECONDS
    ac.ctime = document.getElementById("ctime");
    //ac.chr = document.getElementById("chr");
    ac.cmin = document.getElementById("cmin");
    ac.csec = document.getElementById("csec");

    // (A2) CREATE TIME PICKER - HR, MIN, SEC
    //ac.thr = ac.createSel(23);
    //document.getElementById("tpick-h").appendChild(ac.thr);
    ac.thm = ac.createSel(10);
    document.getElementById("tpick-m").appendChild(ac.thm);
    ac.thm.addEventListener("change", ac.thmChange)
    //ac.ths = ac.createSel(59);
    //document.getElementById("tpick-s").appendChild(ac.ths);

    // (A3) CREATE TIME PICKER - SET, RESET
    ac.tset = document.getElementById("tset");
    ac.tset.addEventListener("click", ac.set);
    ac.treset = document.getElementById("treset");
    ac.treset.addEventListener("click", ac.reset);

    // (A4) GET ALARM SOUND
    ac.sound = document.getElementById("alarm-sound");

    // (A5) START THE CLOCK
    ac.updateClock(ac.thm.value*60000);
    ac.alarm = null;
    setInterval(ac.tick, 1000);
  },

  // (B) SUPPORT FUNCTION - CREATE SELECTOR FOR HR, MIN, SEC
  createSel : function (max) {
    const choices = [.1,5,10,20,30];
    var selector = document.createElement("select");
    for (let i in choices) { //var i=5; i<=max; i+=5) {
      var opt = document.createElement("option");
      temp = choices[i];//ac.padzero(choices[i]);
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
    if (ac.alarm != null) {

    // Get today's date and time
    var now = new Date().getTime();

    // Find the distance between now and the count down date
    var distance = ac.alarm - now;
    ac.updateClock(distance);

    // (D3) CHECK AND SOUND ALARM
      if (distance <= 0) {
        if (ac.sound.paused) { ac.sound.play(); }
        ac.alarm=null; // stop countdown
        ac.reset();
      }
    }
  },

  updateClock : function (distance) {
    // Time calculations for days, hours, minutes and seconds
    var days = Math.floor(distance / (1000 * 60 * 60 * 24));
    var hours = Math.floor((distance % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60));
    var minutes = Math.floor((distance % (1000 * 60 * 60)) / (1000 * 60));
    var seconds = Math.floor((distance % (1000 * 60)) / 1000);
    var min = ac.padzero(minutes);
    var sec = ac.padzero(seconds);

    // (D2) UPDATE HTML CLOCK
    //ac.chr.innerHTML = hr;
    ac.cmin.innerHTML = min;
    ac.csec.innerHTML = sec;
  },

  // (E) SET ALARM
  set : function () {
    //ac.alarm = ac.thr.value + ac.thm.value + ac.ths.value;
    var now = new Date().getTime();
    ac.alarm = new Date(now + ac.thm.value*60000);
    //ac.thr.disabled = true;
    ac.thm.disabled = true;
    //ac.ths.disabled = true;
    ac.tset.disabled = true;
    ac.treset.disabled = false;
    // green background because we should be counting down
    ac.ctime.style.backgroundColor = "green";

  },

  // (F) RESET ALARM
  reset : function () {
    //if (!ac.sound.paused) { ac.sound.pause(); }
    ac.alarm = null;
    //ac.thr.disabled = false;
    ac.thm.disabled = false;
    //ac.ths.disabled = false;
    ac.tset.disabled = false;
    ac.treset.disabled = true;
    ac.ctime.style.backgroundColor = "red";
    ac.updateClock(ac.thm.value*60000);
  },

  thmChange : function () {
    ac.updateClock(ac.thm.value*60000);
  }
};

// (G) START CLOCK ON PAGE LOAD
window.addEventListener("load", ac.init);
)====="};
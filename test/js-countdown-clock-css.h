const char clockcss[] PROGMEM = {R"=====(
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
  margin: 0 auto;
  max-width: 200px;
  padding: 10px;
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
  font-size: 24px;
  background: rgba(255, 255, 255, 0);
  color: #fff;
  border-radius: 5px;
}
#ctime .text {
  margin-top: 10px;
  color: #ddd;
}

/* (C) TIME PICKER */
#tpick {
  margin: 0 auto;
  max-width: 200px;
  padding: 10px;
  background: #f2f2f2;
  white-space: nowrap;
}
#tpick-h, #tpick-m, #tpick-s {
  display: inline-block;
  width: 100%;
}
#tpick select {
  box-sizing: padding-box;
  width: 100%;
  text-align: center;
  font-size: 1.2em;
  font-weight: bold;
  margin: 20px 0;
}
#tset {
  box-sizing: padding-box;
  width: 100%;
  height: 3em;
  background: #0c9113;
  color: #fff;
  border: 0;
  cursor: pointer;
}
#treset {
  box-sizing: padding-box;
  width: 100%;
  height: 3em;
  background: #ff0000;
  color: #fff;
  border: 0;
  cursor: pointer;
}
#tset:disabled, #treset:disabled {
  background: #aaa;
  color: #888;
}
)====="};
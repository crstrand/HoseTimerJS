<?php
// look for the file start.txt and read date/time out of it if it exists
function tRemainGet()
{
  // default values to zero in case something goes wrong reading the values from the files
  $tStart='0';
  $tStop='0';
  
  $filename='tStop.txt';
  // if tStop.txt does not exist, then the timer is not running
  if(is_readable($filename)) // timer running so update tStop.txt
  {
    $handle = fopen($filename, "r");
    if ($handle)
    {
      if (($tStop = fgets($handle, 4096)) !== false)
      {
        // only text in file should be start time
        //echo "Found tStop $tStop in file $filename"."\r\n";
      }
      else
      {
        echo "Error: unexpected fgets() fail"."\r\n";
      }
      fclose($handle);
    }
  }
  
  $tRemain = $tStop - time();
  if($tRemain<=0)
  {
    $tRemain=0;
    unlink("tStart.txt");
    unlink("tStop.txt");
  }
  return $tRemain;
}

// tRemain units = seconds
function tRemainSet($tRemain='0')
{
  // if tRemain>0 timer is running or will be
  if($tRemain>"0")
  {
    // start or update timer
    $filename='tStart.txt';
    // if tStart.txt does not exist, then the timer is not running and we want to tstart it
    if(is_readable($filename)) // timer running so update tStop.txt
    {
      $handle = fopen($filename, "r");
      if ($handle)
      {
        if (($tStart = fgets($handle, 4096)) !== false)
        {
          // only text in file should be start time
          //echo "Found tStart $tStart in file $filename"."\r\n";
        }
        else
        {
          echo "Error: unexpected fgets() fail"."\r\n";
        }
        fclose($handle);
      }
      // tStop = tStart + tRemain
      $tStop = $tStart + $tRemain;
      // save tStop
      $filename='tStop.txt';
      // write tStart value to file
      $handle = fopen($filename, "w");
      if ($handle)
      {
        if (fwrite($handle, $tStop) !== false) 
        {
          // only text in file should be tStop
          //echo "tStop set to ".$tStop."\r\n";
        }
        else
        {
          echo "Error: unexpected fwrite() fail"."\r\n";
        }				
        fclose($handle);
      }

    }
    else // timer not running. start it and create tStart.txt and tStop.txt
    {
      $tStart = time(); // unix timestamp in seconds
      $tStop = $tStart + $tRemain;
      // save tStart.txt and tStop.txt
      $filename='tStart.txt';
      // write tStart value to file
      $handle = fopen($filename, "w");
      if ($handle)
      {
        if (fwrite($handle, $tStart) !== false) 
        {
          // only text in file should be tStart
          //echo "tStart set to ".$tStart."\r\n";
        }
        else
        {
          echo "Error: unexpected fwrite() fail"."\r\n";
        }				
        fclose($handle);
      }
      $filename='tStop.txt';
      // write tStop value to file
      $handle = fopen($filename, "w");
      if ($handle)
      {
        if (fwrite($handle, $tStop) !== false) 
        {
          // only text in file should be tStop
          //echo "tStop set to ".$tStop."\r\n";
        }
        else
        {
          echo "Error: unexpected fwrite() fail"."\r\n";
        }				
        fclose($handle);
      }
    }
  }
  else
  {
    // stop the timer (deleted tStart.txt)
    unlink("tStart.txt");
    unlink("tStop.txt");
  }
}

function detectRequestBody() {
  $rawInput = fopen('php://input', 'r');
  $tempStream = fopen('php://temp', 'r+');
  stream_copy_to_stream($rawInput, $tempStream);
  rewind($tempStream);

  return $tempStream;
}

if ($_SERVER["REQUEST_METHOD"] == "POST") {
  // collect value of input field
  $tRemain = $_POST['tRemain'];
  //echo detectRequestBody()."\r\n";

  if ($tRemain>="0") {
    // set remain time to what was sent
    tRemainset($tRemain);
    echo "tRemain set to ".$tRemain."\r\n";
  } else {
    echo "tRemain is empty";
  }
}
else if($_SERVER["REQUEST_METHOD"] == "GET") {
  // calculate seconds between start and stop times
  echo tRemainGet();
}
?>

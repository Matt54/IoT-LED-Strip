function setupPage(){
  //alert('Message From Arduino, ' + message);

  document.getElementById("timing").value = parseInt(timing);
  document.getElementById("mod").value = parseInt(mod);
  document.getElementById("programNumber").value = parseInt(programNumber);
  document.getElementById("brightness").value = parseInt(brightness);

  if(parseInt(isAudioReactive) > 0){
    document.getElementById("audioReactive").checked = true;
  }
  else{
    document.getElementById("audioReactive").checked = false;
  }

  if(parseInt(isOn) > 0){
    document.getElementById("powerOn").checked = true;
    document.getElementById("togglePower").innerHTML = "Power On";
    document.getElementById('togglePower').style.backgroundColor = 'Green';
  }
  else{
    document.getElementById("powerOn").checked = false;
    document.getElementById("togglePower").innerHTML = "Power Off";
    document.getElementById('togglePower').style.backgroundColor = 'Red';
  }

}

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

}

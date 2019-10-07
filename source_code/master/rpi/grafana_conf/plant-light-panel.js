<style>
.slidecontainer {
  width: 250px;
}

.slider {
  -moz-apeearance: none;
  width: 100%;
  height: 25px;
  background: #282222;
  box-shadow: 0px 0px 10px 2px #7f7f7f;
  outline: none;
  opacity: 0.7;
  -webkit-transition: .2s;
  -moz-transition: 2s;
  transition: opacity .2s;
  border-radius: 4px;
}

.slider:hover {
  opacity: 1;
}

.slider::-moz-slider-thumb {
  idth: 25px;
  height: 25px;
  background: #282222;
  cursor: pointer;
  border-radius: 4px;
  
  border:2px solid white;
	box-sizing:border-box!important;
}

.slider::-moz-range-thumb {
  width: 25px;
  height: 25px;
  cursor: pointer;
}

</style>

<script>

function send_rest_command(command) {
  const Http = new XMLHttpRequest();
  const server_ip = location.host.split(":", 1)[0];
  const rest_api_port = '5000';
  const url='http://' + server_ip + ':' + rest_api_port + command;
  Http.open("GET", url);
  Http.send();
}

function plant_light_on() {
  const rest_command = '/blue_light_intensity/0/20/';
  send_rest_command(rest_command);
}

function plant_light_off() {
  const rest_command = '/blue_light_intensity/0/0/';
  send_rest_command(rest_command);
}

var slider = document.getElementById("duty_slider");
var output = document.getElementById("duty_val");

output.innerHTML = slider.value;

slider.oninput = function() {
  output.innerHTML = this.value;
}

function set_duty() {
  
  const rest_command = '/blue_light_intensity/0/' + slider.value + '/';
  send_rest_command(rest_command);
}

</script>

<center>

<h1>Plant Light</h1>

<h4>
<button onClick="plant_light_on();" style="border-color:green; background: #282222; border-radius: 4px; margin-top:15px;">ON</button>
<button onClick="plant_light_off();" style="border-color:red; background: #282222; border-radius: 4px; margin-top:15px;">OFF</button>
</h4>

<h4>
<div class="slidecontainer">
  <input type="range" min="0" max="100" value="50" class="slider" id="duty_slider" onmouseup="set_duty();" ontouchend="set_duty();" style="border-radius: 4px; margin-top:15px;">
  <p style="margin-top:15px;">Duty: <span id="duty_val"></span> [%]</p>
</div>
</h4>

</center>

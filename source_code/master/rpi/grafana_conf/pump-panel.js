<style>

.pump_id_text {
  -webkit-appearance: none;
  width: 100px;
  min-height: 27px;
  background: #282222;
  box-shadow: 0px 0px 5px 5px #7f7f7f;
  outline: none;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s;
  border-radius: 4px;
  margin-right: 10px;
  text-align: center;
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

var pump_id = document.getElementById("pump_id_input");

function pump_start() {
  const rest_command = '/pump_start/' + pump_id.value + '/';
  send_rest_command(rest_command);
}

function pump_stop() {
  const rest_command = '/pump_stop/' + pump_id.value + '/';
  send_rest_command(rest_command);
}

</script>

<center>
<h1>Pumps</h1>
<br>
<div>
  <input id="pump_id_input" type="number" class="pump_id_text" name="PumpID" placeholder="Pump ID">
  <button id="pump_start" onClick=pump_start() style="color: white; width: 50px; background: #282222; border-radius: 4px; border-color: green">Start</button> 
  <button id="pump_stop" onClick=pump_stop() style="color: white; width: 50px; background: #282222; border-radius: 4px; border-color: red">Stop</button>
</div>
</center>

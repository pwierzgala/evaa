<script>
function send_rest_command(command) {
  const Http = new XMLHttpRequest();
  const server_ip = location.host.split(":", 1)[0];
  const rest_api_port = '5000';
  const url='http://' + server_ip + ':' + rest_api_port + command;
  Http.open("GET", url);
  Http.send();
}

function lift_up() {
  const rest_command = '/light_height/0/65280/';
  send_rest_command(rest_command);
}

function lift_down() {
  const rest_command = '/light_height/0/65535/';
  send_rest_command(rest_command);
}

function lift_stop() {
  const rest_command = '/light_height/0/0/';
  send_rest_command(rest_command);
}
</script>

<center>
  <h1>Elevator</h1>
  <br>
  <h2>
    <button id="move_up" onClick=lift_up() style="color: white; width: 50px; background: #282222; border-radius: 4px;">&#8593;</button>
    <br>
    <button id="move_stop" onClick=lift_stop() style="color: white; width: 50px; background: #282222; border-radius: 4px;">Stop</button>
    <br>
    <button id="move_down" onClick=lift_down() style="color: white; width: 50px; background: #282222; border-radius: 4px;">&#8595;</button>
    <br>
  </h2>
</center>

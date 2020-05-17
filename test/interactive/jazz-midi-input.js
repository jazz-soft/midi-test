var readline = require('readline');
var mt = require('../..');
var jazz = require('jazz-midi');
var portname = process.argv[2];
if (typeof portname == 'undefined') portname = 'VIRTUAL MIDI PORT';

console.log('midi-test v.' + mt.version);
console.log('jazz-midi v.' + jazz.version);

if (exists(portname)) {
  console.log('Port `' + portname + '` already exists!');
  return;
}

var src = mt.MidiSrc(portname);
if (!src.connect()) {
  console.log('Cannot connect `' + portname + '`!');
  return;
}
console.log('Connected `' + portname + '`...');

var midi = new jazz.MIDI();
var name = midi.MidiInOpen(portname, function(t, msg){
  console.log('Received:', msg);
});
if (name != portname) {
  console.log('Cannot open `' + portname + '`!');
  midi.MidiInClose(name);
  return;
}
console.log('Opened `' + portname + '`...');

console.log('Enter MIDI sequence or Ctrl-C to terminate...');
console.log('Multiple messages can be separated by comma,');
console.log('e.g.: c0 10, 90 40 7f');

var rl = readline.createInterface({ input: process.stdin, output: process.stdout });
rl.on('line', function(line){
  var i, j, chunk, msg, x;
  var group = [];
  var data = line.split(/,\s*/);
  for (i = 0; i < data.length; i++) {
    if (data[i] == '') continue;
    chunk = data[i].split(/\s+/);
    msg = [];
    for (j = 0; j < chunk.length; j++) {
      if (chunk[j] == '') continue;
      x = parseInt(chunk[j], 16);
      if (x >= 0 && x <= 255) msg.push(x);
      else {
        console.log('Invalid hex MIDI value:', chunk[j]);
        return;
      }
    }
    if (msg.length) group.push(msg);
  }
  if (!group.length) return;
  for (i = 0; i < group.length; i++) {
    data = [];
    msg = group[i];
    for (j = 0; j < msg.length; j++) {
      data.push(('0' + msg[j].toString(16)).substr(-2));
    }
    console.log('Sending:', data.join(' '));
    src.emit(msg);
  }
}).on('SIGINT', function() {
  midi.MidiInClose(portname);
  console.log('Closed `' + portname + '`...');
  src.disconnect();
  console.log('Disconnected `' + portname + '`...');
  console.log('Thank you!');
  process.exit();
});

function exists(name) {
  var ports = jazz.MidiInList();
  for (var i = 0; i < ports.length; i++) if (ports[i] == name) return true;
  return false;
}

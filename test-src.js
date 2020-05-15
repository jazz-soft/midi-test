var readline = require('readline');
var mt = require('.');
console.log('midi-test v.' + mt.version);
var src = mt.MidiSrc('VIRTUAL MIDI-In');
console.log('Opening ' + src.name + ':', src.connect());
console.log('Enter MIDI sequence or Ctrl-C to terminate...');
var rl = readline.createInterface({ input: process.stdin, output: process.stdout });
rl.on('line', function(line){
  var data = line.split(/\s+/);
  var msg = [];
  var i, x;
  for (i = 0; i < data.length; i++) {
    if (data[i] == '') continue;
    x = parseInt(data[i], 16);
    if (x >= 0 && x <= 255) msg.push(x);
    else {
      console.log('Invalid hex MIDI value:', data[i]);
      return;
    }
  }
  if (!msg.length) return;
  data = [];
  for (i = 0; i < msg.length; i++) {
    data.push(('0' + msg[i].toString(16)).substr(-2));
  }
  console.log('Sending:', data.join(' '));
  src.emit(msg);
});

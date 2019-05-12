const mt = require('.');
var send = [ [0x90, 0x40, 0x7f], [0x80, 0x40, 0x00] ];
var count = 0;
var src = mt.MidiSrc('VIRTUAL MIDI-In');
console.log('Opening ' + src.name + ':', src.connect());
console.log('Press <Enter> to continue...');
process.stdin.on('data', function(s) {
  if (count < send.length) {
    console.log('Sending:', send[count]);
    src.emit(send[count]);
    count++;
  }
  else {
    console.log('Closing ' + src.name + ':', src.disconnect());
    console.log('Done!');
    process.exit(0);
  }
});

const mt = require('.');
var dst = mt.MidiDst('VIRTUAL MIDI-Out');
console.log('Opening ' + dst.name + ':', dst.connect());
dst.receive = function(msg) {
  console.log('received:', msg);
};
console.log('Press <Enter> to exit...');
process.stdin.on('data', function(s) {
  console.log('Closing ' + dst.name + ':', dst.disconnect());
  console.log('Done!');
  process.exit(0);
});

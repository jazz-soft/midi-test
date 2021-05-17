var mt = require('.');
console.log('midi-test v.' + mt.version);
var dst = mt.MidiDst('VIRTUAL MIDI-Out');
console.log('Opening ' + dst.name + ':', dst.connect());
if (!dst.connected) {
  console.log('Could not open ' + dst.name + '!');
  process.exit(0);
}
dst.receive = function(msg) {
  console.log('received:', msg);
};
console.log('Press <Enter> to exit...');
process.stdin.on('data', function() {
  console.log('Closing ' + dst.name + ':', dst.disconnect());
  console.log('Done!');
  process.exit(0);
});

/** MIDI Source -- a virtual MIDI-In port */
interface MidiSrc {
  /** Port name */
  readonly name: string;
  /** Connect the port */
  readonly connect: () => boolean;
  /** Disconnect the port */
  readonly disconnect: () => boolean;
  /** Emit MIDI message */
  readonly emit: (message: number[]) => void;
}
declare namespace MidiSrc {
  interface Constructor {
    /** Create new MidiDst object */
    new (name: string): MidiSrc;
    /** Create new MidiDst object */
    (name: string): MidiSrc;
  }
}

/** MIDI Destination -- a virtual MIDI-Out port */
interface MidiDst {
  /** Port name */
  readonly name: string;
  /** Connect the port */
  readonly connect: () => boolean;
  /** Disconnect the port */
  readonly disconnect: () => boolean;
  /** User-defined MIDI message handler */
  receive: (message?: number[]) => void;
}
declare namespace MidiDst {
  interface Constructor {
    /** Create new MidiDst object */
    new (name: string): MidiDst;
    /** Create new MidiDst object */
    (name: string): MidiDst;
  }
}

interface MidiTest {
  readonly MidiSrc: MidiSrc.Constructor;
  readonly MidiDst: MidiDst.Constructor;
}
declare const mt: MidiTest;

export = mt;

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

interface MidiSrcConstructor {
  /** Create new MidiSrc object */
  new (name: string): MidiSrc;
  /** Create new MidiSrc object */
  (name: string): MidiSrc;
}

interface MidiDstConstructor {
  /** Create new MidiDst object */
  new (name: string): MidiDst;
  /** Create new MidiDst object */
  (name: string): MidiDst;
}

export const MidiSrc: MidiSrcConstructor;
export const MidiDst: MidiDstConstructor;

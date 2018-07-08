{
  "targets": [
    {
      "target_name": "miditest",
      "sources": [ "src/miditest.cpp" ],
      "conditions":
      [
        [
          "OS=='win'",
          {
            "sources": [ "src/miditest_stub.cpp" ]
          }
        ],
        [
          "OS=='mac'",
          {
            "sources":[  "src/miditest_mac.cpp" ],
            "link_settings":
            {
              "libraries": [ "CoreMIDI.framework" ]
            }

          }
        ],
        [
          "OS=='linux'",
          {
            "sources": [ "src/miditest_stub.cpp" ]
          }
        ]
      ]
    }
  ]
}

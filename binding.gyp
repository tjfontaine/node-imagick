{
  'targets': [
    {
      'target_name': 'imagick',
      'variables': {
        'wand-config': 'Wand-config',
        #'mpp-config': 'Magick++-config',
      },
      'sources': [
        'src/imagick.cc',
        'src/command.cc',
        'src/compare.c',
        #'src/image.cc',
        #'src/geometry.cc',
        #'src/blob.cc',
      ],
      'include_dirs': [ 'include' ],
      'cflags': [
        '<!@(<(wand-config) --cppflags)',
        #'<!@(<(mpp-config) --cppflags)',
      ],
      'libraries': [
        '<!@(<(wand-config) --libs)',
        #'<!@(<(mpp-config) --libs)',
      ],
      'ldflags': [
        '<!@(<(wand-config) --ldflags)',
        #'<!@(<(mpp-config) --ldflags)',
      ],
      'xcode_settings': {
        'OTHER_CFLAGS': [
          '<!@(<(wand-config) --cppflags)',
          #'<!@(<(mpp-config) --cppflags)',
        ],
        'OTHER_LDFLAGS': [
          '<!@(<(wand-config) --ldflags)',
          #'<!@(<(mpp-config) --ldflags)',
        ],
      },
    }
  ]
}

from fabricate import *

sources = [
            # Magic Files
            'startup_gcc',
            'main',

            # HAL files
            'hal_uart',
          ]

def build():
    compile()
    link()
    prepare()

def compile():
    INCLUDE_PATH = '/media/sf_virtualbox/stellaris_dev/stellaris'

    compile_options = [ '-g',
                        '-mthumb',
                        '-mcpu=cortex-m4',
                        '-mfpu=fpv4-sp-d16',
                        '-mfloat-abi=softfp',
                        '-Os',
                        '-ffunction-sections',
                        '-fdata-sections',
                        '-MD',
                        '-std=c99',
                        '-Wall',
                        '-pedantic',
                        '-DPART_LM4F120H5QR',
                        '-c',
                        '-I' + INCLUDE_PATH,
                        '-DTARGET_IS_BLIZZARD_RA1' ]

    for source in sources:
        run( 'arm-none-eabi-gcc',
             source+'.c',
             compile_options ) #' '.join( compile_options )

def link():

    objects = [s+'.o' for s in sources]

    linker_options = [ '--script=shifty.ld',
                       '--entry=ResetISR',
                       '--output=shifty.out',
                       objects,
                       '--gc-sections' ]


    #arm-none-eabi-ld -T blink.ld --entry ResetISR -o a.out startup_gcc.o blink.o --gc-sections
    run( 'arm-none-eabi-ld', linker_options )

def prepare():
    prepare_options = [  '-O', 'binary', 'shifty.out', 'shifty.bin' ]
    run( 'arm-none-eabi-objcopy', prepare_options )

def clean():
    autoclean()

main()
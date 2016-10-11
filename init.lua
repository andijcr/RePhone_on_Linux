os.shell_linetype(1)
dofile('ortobio.lua')
ortobio_init()

sys.onalarm(ortobio)
ortobio()


def readFile(filename):
	with open('jenkins/' + filename) as f:
		lines = f.readlines()
		(text, data, bss, dec, hex , filename) = lines[1].split()
		return {'text' : int(text), 'data' : int(data), 'bss' : int(bss), 'dec' : int(dec), 'hex' : hex}

allDisabled = readFile('allDisabled.size')
oneEnabled = readFile('oneEnabled.size')
twoEnabled = readFile('twoEnabled.size')
threeEnabled = readFile('threeEnabled.size')
fourEnabled = readFile('threeEnabled.size')
fiveEnabled = readFile('threeEnabled.size')
sixEnabled = readFile('threeEnabled.size')
sevenEnabled = readFile('threeEnabled.size')
eightEnabled = readFile('threeEnabled.size')

oneTextSize = oneEnabled['text'] - allDisabled['text']
twoTextSize = twoEnabled['text'] - oneEnabled['text']
threeTextSize = threeEnabled['text'] - twoEnabled['text']
fourTextSize = fourEnabled['text'] - threeEnabled['text']
fiveTextSize = fiveEnabled['text'] - fourEnabled['text']
sixTextSize = sixEnabled['text'] - fiveEnabled['text']
sevenTextSize = sevenEnabled['text'] - sixEnabled['text']
eightTextSize = eightEnabled['text'] - sevenEnabled['text']

with open('outputDiff.txt', 'w') as f:
	f.write('no serial enabled, enabled 1, enabled 2, enabled 3, enabled 4, enabled 5, enabled 6, enabled 7, enabled 8\n')
	f.write(str(allDisabled['text']) + ', ' + str(oneTextSize) + ', ' + str(twoTextSize) + ', ' + str(threeTextSize) + ', ' + str(fourTextSize) + ', ' + str(fiveTextSize) + ', ' + str(sixTextSize) + ', ' + str(sevenTextSize) + ', ' + str(eightTextSize))

with open('output.txt', 'w') as f:
	f.write('no serial enabled, enabled 1, enabled 2, enabled 3, enabled 4, enabled 5, enabled 6, enabled 7, enabled 8\n')
	f.write(str(allDisabled['text']) + ', ' + str(oneEnabled['text']) + ', ' + str(twoEnabled['text']) + ', ' + str(threeEnabled['text']) + ', ' + str(fourEnabled['text']) + ', ' + str(fiveEnabled['text']) + ', ' + str(sixEnabled['text']) + ', ' + str(sevenEnabled['text']) + ', ' + str(eightEnabled['text']))

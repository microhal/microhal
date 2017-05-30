def readFile(filename):
	with open(filename) as f:
		lines = f.readlines()
		(text, data, bss, dec, hex , filename) = lines[1].split()
		return {'text' : int(text), 'data' : int(data), 'bss' : int(bss), 'dec' : int(dec), 'hex' : hex}

allDisabled = readFile('allDisabled.size')
oneEnabled = readFile('oneEnabled.size')
twoEnabled = readFile('twoEnabled.size')
threeEnabled = readFile('threeEnabled.size')
fourEnabled = readFile('fourEnabled.size')
fiveEnabled = readFile('fiveEnabled.size')
sixEnabled = readFile('sixEnabled.size')
sevenEnabled = readFile('sevenEnabled.size')
eightEnabled = readFile('eightEnabled.size')

oneTextSize = oneEnabled['text'] - allDisabled['text']
twoTextSize = twoEnabled['text'] - oneEnabled['text']
threeTextSize = threeEnabled['text'] - twoEnabled['text']
fourTextSize = fourEnabled['text'] - threeEnabled['text']
fiveTextSize = fiveEnabled['text'] - fourEnabled['text']
sixTextSize = sixEnabled['text'] - fiveEnabled['text']
sevenTextSize = sevenEnabled['text'] - sixEnabled['text']
eightTextSize = eightEnabled['text'] - sevenEnabled['text']

with open('output_Diff_text.txt', 'w') as f:
	f.write('no serial enabled, enabled 1, enabled 2, enabled 3, enabled 4, enabled 5, enabled 6, enabled 7, enabled 8\n')
	f.write(str(allDisabled['text']) + ', ' + str(oneTextSize) + ', ' + str(twoTextSize) + ', ' + str(threeTextSize) + ', ' + str(fourTextSize) + ', ' + str(fiveTextSize) + ', ' + str(sixTextSize) + ', ' + str(sevenTextSize) + ', ' + str(eightTextSize))

def saveFile(filename, section):
	with open(filename, 'w') as f:
		f.write('no serial enabled, enabled 1, enabled 2, enabled 3, enabled 4, enabled 5, enabled 6, enabled 7, enabled 8\n')
		f.write(str(allDisabled[section]) + ', ' + str(oneEnabled[section]) + ', ' + str(twoEnabled[section]) + ', ' + str(threeEnabled[section]) + ', ' + str(fourEnabled[section]) + ', ' + str(fiveEnabled[section]) + ', ' + str(sixEnabled[section]) + ', ' + str(sevenEnabled[section]) + ', ' + str(eightEnabled[section]))

saveFile('output_text_size.txt', 'text')
saveFile('output_data_size.txt', 'data')
saveFile('output_bss_size.txt', 'bss')

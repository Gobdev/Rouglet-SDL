from PIL import Image
import os, re

pngFiles = [file for file in os.listdir('.') if re.match(".*\.png$", file)]
images = {}
for file in pngFiles:
	key = re.sub('\..*$', "", file)
	im = Image.open(file) #Can be many different formats.
	pix = im.load()
	x, y = im.size
	lista = []
	if not x == 7 or not y == 7:
		continue
	for i in range(x):
		value = 0;
		for j in range(y):
			if (sum(pix[i, j]) / 3) > 128:
				value += 2**j
		lista.append(hex(value))
	images[key] = lista

f = open('images.c', 'w')
for key in images:
	print "Added image '" + key + "'."
	f.write('const char const ' + key + '[] = {')
	f.write(', '.join(images.get(key)))
	f.write('};\n')
f.close()
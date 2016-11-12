from PIL import Image
import os, re

path = re.match('(.*?)(/src(/images)?)?$', os.getcwd()).group(1)
path += "/src/images/"
pngFiles = [file for file in os.listdir(path) if re.match(".*\.png$", file)]
images = {}
for file in pngFiles:
	key = re.sub('\..*$', "", file)
	im = Image.open(path + file) #Can be many different formats.
	pix = im.load()
	x, y = im.size
	lista = []
	if not x == 7 or not y == 7:
		continue
	for i in range(x):
		value = 0;
		for j in range(y):
			pixel = pix[i, j]
			if isinstance(pixel, tuple):
				if sum(pixel) / len(pixel) > 128:
					value += 2**j
			else:
				if pixel > 128:
					value += 2**j
		lista.append(hex(value))
	images[key] = lista

f = open(path + 'images.c', 'w')
for key in images:
	print "Added image '" + key + "'."
	f.write('const char const ' + key + '[] = {')
	f.write(', '.join(images.get(key)))
	f.write('};\n')
f.close()
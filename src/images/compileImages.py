#! python
from PIL import Image
import os, re

path = re.match('(.*?)(/src(/images)?)?$', os.getcwd()).group(1)
path += "/src/images/"
dirs = [directory for directory in os.listdir(path) if os.path.isdir(path + directory)]
for directory in dirs:
	images = {}
	pngFiles = [file for file in os.listdir(path + directory) if re.match('.*\.png$', file)]
	for file in pngFiles:
		key = re.sub('\..*$', "", file)
		im = Image.open(path + directory + "/" + file) #Can be many different formats.
		pix = im.load()
		x, y = im.size
		lista = [str(x), str(y)]
		for page in range((int)((y - 1) / 8) + 1):
			for i in range(x):
				value = 0;
				for j in range(8):
					if (page * 8 + j >= y):
						break
					pixel = pix[i, page * 8 + j]
					if isinstance(pixel, tuple):
						if sum(pixel) / len(pixel) > 128:
							value += 2**j
					else:
						if pixel > 128:
							value += 2**j
				lista.append(hex(value))
		images[key] = lista

	f = open(path + directory + '.h', 'w')
	print ("Created file '" + path + directory + ".h'.")
	f.write("#pragma once\n")
	for key in images:
		f.write('const unsigned char ' + key + '[' + str(len(images.get(key))) + '] = {')
		f.write(', '.join(images.get(key)))
		f.write('};\n')	
	f.close()

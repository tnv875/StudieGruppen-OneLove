import os
url = "C:\\Users\\Lenovo\\Documents\\GitHub\\StudieGruppen-OneLove\\a4\\Dog_meme"


accept = "*/png"

types = accept.split(sep= ", ")

types_dict = {}
for type_ in types:
	try:
		if ";" in type_:
			key, value = type_.split(sep=";", maxsplit=1)
			variable, value = value.split(sep="=", maxsplit=1)
			types_dict.update({key: value})
		else:
			types_dict.update({type_: "1.1"})
	except:
		next

types_subtypes = {"*": "*", "image": ["jpeg", "png", "svg+xml"], "text": ["html", "plain", "csv", "css"], "application": ["xhtml+xml", "xml"]}
accepted_list = []
for type_subtype in types_dict.keys():
	MIME_type, MIME_subtype = type_subtype.split(sep="/")
	if MIME_type in types_subtypes:
		if (MIME_subtype in types_subtypes[MIME_type]) or MIME_type == "*":
			#Make a list of all matches --> Sorted List
			accepted_list.append(MIME_type+"/"+MIME_subtype)
		else:
			next
	else:
		next
if accepted_list == []:
	print(406)
else:
	final_dict = {}
	for elem in accepted_list:
		value = types_dict[elem]
		final_dict.update({elem: value})
	sorted_final_list_tuples = sorted(final_dict.items(), key= lambda x:x[1], reverse=True)
	
	# Search for file ending in sorted order from URL and self 
	for best_option in sorted_final_list_tuples:
		if best_option[0].split(sep="/")[1] =="*":
			if best_option[0].split(sep="/")[0] == "*":
				for type in types_subtypes:

					for elem in	types_subtypes[type]:
						if os.path.exists(url + "." + elem):
							url = url + "." + elem
							print(url)
			else:
				for elem in	types_subtypes[best_option[0].split(sep="/")[0]]:
					if os.path.exists(url + "." + elem):
						url = url + "." + best_option[0].split(sep="/")[1]
						print(url)
						break		
		elif os.path.exists(url + "." + best_option[0].split(sep="/")[1]):
			url = url + "." + best_option[0].split(sep="/")[1]
			print(url)
			break
		else:
			next
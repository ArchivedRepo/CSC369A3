import sys

if __name__ == "__main__":
    usage = "USAGE: python3 unique_page.py <filename>"
    if len(sys.argv) != 2:
        print(usage)
        exit(1)
    file = open(sys.argv[1])
    unique_pages = []
    for line in file.readlines():
        this_line = line.strip()
        if this_line[:-3] not in unique_pages:
            unique_pages.append(this_line[:-3])
    print("The number of unique pages is: {}".format(len(unique_pages)))

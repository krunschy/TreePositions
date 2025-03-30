wenn man die werte aus dem excel in die .txt copypasted, sind sie durch tab getrennt, was die console nicht lesen kann.
Daher öffnet man dannach die .txt files in VS Code und macht folgendes:
strg + h
oben \t eingeben
regular expression button enablen
unten einmal space drücken
und dann replace all

hab die daten für die distanzen "berichtigt" indem ich doppelte kanten einfach gemittelt hab

what I want is the following algorithm:
First split the trees list into a list called known trees and a list called unknown trees. The first list is just the first 14 entries and unknown trees the rest.
Then I want the following recursive function:
Start with failcount =0
find the lowest number of a tree on the unknown list
add failcount and find the according tree from the unknown list. If you can find it, call it newKnownTree and continue, else return with some simple error message.
take in all elements from the distance array, where the tree number matches tree1 or tree2
Then only select those elements, where tree1 or tree2 is in known trees
if no such two elements exist, call the function again with, but increase the failcount by 1
if they do, find the two other trees in known trees and read their x and y coord

now here's the mathy part, which I'll do later. comment it in the code. trhat this is to do, but for now simply take newKnownTree and put it into the knownTrees list and delete it from the unknownTrees

set failcount to 0
Now check whether the unknownTrees list is empty, and if so return with a success message
else call the function itself with the new unknownTrees list, knownTrees list, and failcount
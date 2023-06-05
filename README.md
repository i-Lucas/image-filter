<h1>C-image-filter</h1>

<br />
<h2>How to run the project: </h2>

<h3>You will need to have Clang installed
Clone the project and run the make filter command in the project directory</h3>

<br />
<br />
<h2>Currently the project has 4 filters:</h3>
<li>grayscale</li>
<li>blur</li>
<li>mirrored</li>
<li>edges</li>

<br />
<br />
<h1>Use:</h1>

```bash
make filter

./filter -flag (b, e, g, r) ./images/select_image.bmp ./outputdir/outputname.bmp
```

<br />
<h1>Example:</h1>

```bash
 ./filter -g ./images/yard.bmp ./output/yard_gray.bmp
```

```
This project is part of my studies in C, don't mind if the code is dirty and heavily commented, I'm still new to this but I already love C
```
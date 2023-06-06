<h1>Simple Image-filter</h1>

<br />
<h2>How to run the project: </h2>

<h3>You will need to have Clang installed</h3>
<h5>Clone the project and run the make filter command in the project directory or execute ./run.sh</h5>

<h2>Currently the project has 4 filters:</h3>
<li>grayscale <strong>flag -g</strong></li>
<li>blur <strong>flag -g</strong></li>
<li>mirrored <strong>flag -r</strong></li>
<li>edges <strong>flag -e</strong></li>
<li>sepia <strong>flag -s</strong></li>

<br />
<h1>Use:</h1>

<h2>give permission for the run.sh file with the command:</h2>

 <br />

```bash
chmod +x run.sh
```
 <br />

```bash
./filter -flag ./images/select_image.bmp ./outputdir/outputname.bmp
```

<h1>Example:</h1>

```bash
 ./filter -g ./images/yard.bmp ./output/yard_gray.bmp
```

```
This project is part of my studies in C, 
don't mind if the code is dirty and heavily commented,
I'm still new to this but I already love C
```

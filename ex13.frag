#version 130
// Inter-image processing
// Hexagon Pixelation based off of blog at https://pixelero.wordpress.com/2008/06/12/pixel-bender-hexcells/


uniform int       mode;
uniform float     frac;
uniform sampler2D img0;
uniform sampler2D img1;

#define HEX_RATIO 0.02
#define HEX_SIZE (((frac*2)) * HEX_RATIO/sqrt(3.))

// repeat color for size of the repeated hexagon pixel
vec2 hexCoord(ivec2 hexIndex) {
	int i = hexIndex.x;
	int j = hexIndex.y;
	vec2 returnValue;
	returnValue.x = i * HEX_SIZE;
	returnValue.y = j * HEX_RATIO + (i%2) * HEX_RATIO/2.;
	return returnValue;
}

// determine the size of the repeated hexagon pixel
ivec2 hexIndex(vec2 coord) {
	ivec2 returnValue;
	float x = coord.x;
	float y = coord.y;
	int it = int(floor(x/HEX_SIZE));
	float yts = y - float(it%2) * HEX_RATIO/2.0;
	int jt = int(floor((1.0/HEX_RATIO) * yts));
	float xt = x - it * HEX_SIZE;
	float yt = yts - jt * HEX_RATIO;
	int deltaj = (yt > HEX_RATIO/2.)? 1:0;
	
	// Set up a conditional value to check against
	float fcond = HEX_SIZE * (2.0/3.0) * abs(0.5 - yt/HEX_RATIO);

	if (xt > fcond) {
		// 
		returnValue.x = it;
		returnValue.y = jt;
	}
	else {
		// 
		returnValue.x = it - 1;
		returnValue.y = jt - (returnValue.x%2) + deltaj;
	}

	return returnValue;
}

void main()
{
   vec4 pix0 = texture2D(img0,gl_TexCoord[0].st);
   vec4 pix1 = texture2D(img1,gl_TexCoord[0].st);
      //  First image
      if (mode == 0 )
         gl_FragColor = pix0;
      //  Second image
      else if (mode == 1)
         gl_FragColor = pix1;
      //  Transition
      else if (mode ==  2)
         gl_FragColor = mix(pix0,pix1,frac);
      //  Absolute difference
      else if (mode ==  3)
         gl_FragColor = abs(pix1-pix0);
      //  False color
      else if (mode ==  4)
         gl_FragColor = vec4(length(pix0.rgb),0,length(pix1.rgb),1);
      // Normal Pixelate
      else if (mode == 5) {
      	// pixel x, y is duplicated into x+dx, y+dy rectangle
      	// dx, dy determined by desired pixel count, set by
      	// slider
      	if (frac != 0) {
			 float dx = (frac*10.0)*(1./512.);
			 float dy = (frac*15.0)*(1./512.);
			 vec2 coord = vec2(dx*floor(gl_TexCoord[0].x/dx),
						       dy*floor(gl_TexCoord[0].y/dy));
			 gl_FragColor = texture2D(img0, coord);
			 }
		else gl_FragColor = pix0;
      }
      // Hexagon Pixels
      else if (mode == 6) {
      	if (frac > 0) {
			vec2 xy = gl_TexCoord[0].xy;
			ivec2 hexIx = hexIndex(xy);
			vec2 hexXy = hexCoord(hexIx);
			vec4 fcol = texture2D(img1, hexXy);
			gl_FragColor = fcol;
		}
		else gl_FragColor = pix1;
      }
      else
         discard;
}

///////////////////////////////////////////////////////////////////////////////
// teapot.h
// ========
// vertex, normal and index array for teapot model
// (6320 polygons, 3241 vertices, 3259 normals)
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2005-10-05
// 
// UPDATED: 2016-08-25  (Y. Hu: Microsoft Visual Studio 2015 + Platform toolset in Visual Studio 2015; OpenHatics-V3.2.2; System configuration - Windows 7 + 16:9 screen ratio) 
// UPDATED: 2018-08-02  (Y. Hu: Microsoft Visual Studio 2017 + Platform toolset in Visual Studio 2017; OpenHatics-V3.2.2; System configuration - Windows 7 + 16:9 screen ratio) 
//
// 3D model is converted by the PolyTrans from Okino Computer Graphics, Inc.
//
// Bounding box of geometry = (-3,0,-2) to (3.434,3.15,2).
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
// draw teapot using absolute pointers to indexed vertex array.
///////////////////////////////////////////////////////////////////////////////


/*
void drawTeapot()
{
	float shininess = 15.0f;

	// set specular and shiniess using glMaterial (gold-yellow)
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess); // range 0 ~ 128
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);

	// set ambient and diffuse color using glColorMaterial (gold-yellow)
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColor3fv(diffuseColor);

	// start to render polygons
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glNormalPointer(GL_FLOAT, 0, teapotNormals);
	glVertexPointer(3, GL_FLOAT, 0, teapotVertices);

	glDrawElements(GL_TRIANGLE_STRIP, 12, GL_UNSIGNED_SHORT, &teapotIndices[0]);
	
	glDisableClientState(GL_VERTEX_ARRAY);	// disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);	// disable normal arrays
}


///////////////////////////////////////////////////////////////////////////////
// create a display list for teapot
// Call creatTeapotDL() once to create a DL. createTeapotDL() will return a ID
// of display list. Use this ID to render later, glCallList(id).
//
// Since display lists are part of server state, the client state and commands
// cannot be stored in display list. Therefore, glEnableClientState,
// glDisableClientState, glVertexPointer, and glNormalPointer cannot be inside
// a display list. Above client calls must be reside outside of glNewList() and
// glEndList() function.

GLuint createTeapotDL()
{
	GLuint id = 0;
	float shininess = 15.0f;
	//float diffuseColor[3] = {0.929524f, 0.796542f, 0.178823f};
	//float specularColor[4] = {1.00000f, 0.980392f, 0.549020f, 1.0f};

	// generate one display list
	id = glGenLists(1);
	if(!id) return id;  // failed to create a list, return 0

	// bind vertex and normal pointers
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 0, teapotNormals);
	glVertexPointer(3, GL_FLOAT, 0, teapotVertices);

	// store drawing function in the display list =============================
	glNewList(id, GL_COMPILE);

	// set specular and shiniess using glMaterial (gold-yellow)
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess); // range 0 ~ 128
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);

	// set ambient and diffuse color using glColorMaterial (gold-yellow)
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColor3fv(diffuseColor);

	// start to render polygons
	glDrawElements(GL_TRIANGLE_STRIP, 12, GL_UNSIGNED_SHORT, &teapotIndices[0]);
		
	// ...

	glDrawElements(GL_TRIANGLE_STRIP, 7, GL_UNSIGNED_SHORT, &teapotIndices[6898]);

	glEndList();	//=========================================================

	glDisableClientState(GL_VERTEX_ARRAY);	// disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);	// disable normal arrays

	return id;  // use this for drawing
}



///////////////////////////////////////////////////////////////////////////////
// draw teapot using only offset instead of absolute pointers.
// The caller must bind buffer ids and set the starting offset before call this
// functions. (glBindBufferARB, glVertexPointer, glNormalPointer, glIndexPointer)
///////////////////////////////////////////////////////////////////////////////
void drawTeapotVBO()
{
	float shininess = 15.0f;
	//float diffuseColor[3] = {0.929524f, 0.796542f, 0.178823f};
	//float specularColor[4] = {1.00000f, 0.980392f, 0.549020f, 1.0f};

	// set specular and shiniess using glMaterial (gold-yellow)
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess); // range 0 ~ 128
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);

	// set ambient and diffuse color using glColorMaterial (gold-yellow)
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColor3fv(diffuseColor);

	// start to render polygons
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	// use only offset here instead of absolute pointer addresses
	glDrawElements(GL_TRIANGLE_STRIP, 12, GL_UNSIGNED_SHORT, (GLushort*)0+0);
	
	glDrawElements(GL_TRIANGLE_STRIP, 7, GL_UNSIGNED_SHORT, (GLushort*)0+6898);

	glDisableClientState(GL_VERTEX_ARRAY);	// disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);	// disable normal arrays
}
*/
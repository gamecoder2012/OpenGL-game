// Thanks Antti for sending me this File
// I am unsure who the original author of this file is, use at your own risk
//
// Hosted by opengltutorials.com as a courtesy to the internet community
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "glm.cpp"
//#include "chess.cpp"
#include <cstring>
#include <iostream>
#include <cmath>
static float ypoz = 0, zpoz = 0,tzpos=0.0,txpos=0.0,typos=0,hel_x=0.0,hel_z=0.0,zoom=16.0;
int move=1,n_tiles=10;
// You need to declare your self a pointer to the model
// This is where the data will be loaded
GLMmodel* pmodel1 = NULL;
GLMmodel* pmodel2 = NULL;
GLMmodel* pmodel3 = NULL;
float frangle=0.0;
typedef struct tiles
{
	float x1,z1,x2,z2,height;
	int type,draw;
}ground;

GLvoid glmDraw1(GLMmodel* model, GLuint mode,char *drawonly)
{
	//	cout<<"tejas ki maa";
	static GLuint i;
	static GLMgroup* group;
	static GLMtriangle* triangle;
	static GLMmaterial* material;
	GLuint IDTextura;

	assert(model);
	assert(model->vertices);

	/* do a bit of warning */
	if (mode & GLM_FLAT && !model->facetnorms) {
		printf("glmDraw() warning: flat render mode requested "
				"with no facet normals defined.\n");
		mode &= ~GLM_FLAT;
	}
	if (mode & GLM_SMOOTH && !model->normals) {
		printf("glmDraw() warning: smooth render mode requested "
				"with no normals defined.\n");
		mode &= ~GLM_SMOOTH;
	}
	if (mode & GLM_TEXTURE && !model->texcoords) {
		printf("glmDraw() warning: texture render mode requested "
				"with no texture coordinates defined.\n");
		mode &= ~GLM_TEXTURE;
	}
	if (mode & GLM_FLAT && mode & GLM_SMOOTH) {
		printf("glmDraw() warning: flat render mode requested "
				"and smooth render mode requested (using smooth).\n");
		mode &= ~GLM_FLAT;
	}
	if (mode & GLM_COLOR && !model->materials) {
		printf("glmDraw() warning: color render mode requested "
				"with no materials defined.\n");
		mode &= ~GLM_COLOR;
	}
	if (mode & GLM_MATERIAL && !model->materials) {
		printf("glmDraw() warning: material render mode requested "
				"with no materials defined.\n");
		mode &= ~GLM_MATERIAL;
	}
	if (mode & GLM_COLOR && mode & GLM_MATERIAL) {
		printf("glmDraw() warning: color and material render mode requested "
				"using only material mode.\n");
		mode &= ~GLM_COLOR;
	}
	if (mode & GLM_COLOR)
		glEnable(GL_COLOR_MATERIAL);
	else if (mode & GLM_MATERIAL)
		glDisable(GL_COLOR_MATERIAL);
	if (mode & GLM_TEXTURE) {
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	/* perhaps this loop should be unrolled into material, color, flat,
	   smooth, etc. loops?  since most cpu's have good branch prediction
	   schemes (and these branches will always go one way), probably
	   wouldn't gain too much?  */

	IDTextura = -1;
	group = model->groups;
	while (group) 
	{
		glPushMatrix();
		if(strcmp(group->name," right_leg")==0)
		{
			//			cout<<"maa ki choot";
			//		glTranslatef(0,-0.2,0.5);
			glRotatef(frangle,1,0,0);
			//		glTranslatef(0,+0.2,-0.5);

		}
		else if(strcmp(group->name," left_leg")==0)
		{
			//			cout<<"maa ki choot";
			//		glTranslatef(0,-0.2,0.5);
			glRotatef(-frangle,1,0,0);
			//			glTranslatef(0,+0.2,-0.5);

		}
		else if(strcmp(group->name," right_hand")==0)
		{
			//			cout<<"maa ki choot";
			glTranslatef(0,0.6,0);
			glRotatef(-frangle,1,0,0);
			glTranslatef(0,-0.6,0);

		}
		else if(strcmp(group->name," left_hand")==0)
		{
			//			cout<<"maa ki choot";
			glTranslatef(0,0.6,0);
			glRotatef(frangle,1,0,0);
			glTranslatef(0,-0.6,0);

		}
		else if(strcmp(group->name," left_arm")==0)
		{
			//			cout<<"maa ki choot";
			glTranslatef(0,1.6,0);
			glRotatef(frangle,1,0,0);
			glTranslatef(0,-1.6,0);

		}
		if (drawonly)
			if (strcmp(group->name,drawonly))
			{
				group=group->next;
				continue;
			}		

		material = &model->materials[group->material];
		if (material)
			IDTextura = material->IDTextura;
		else IDTextura=-1;

		if (mode & GLM_MATERIAL) 
		{            
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material->ambient);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material->diffuse);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material->specular);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->shininess);
		}

		if (mode & GLM_TEXTURE) 
		{				
			if(IDTextura == -1)
				glBindTexture(GL_TEXTURE_2D, 0);
			else
				glBindTexture(GL_TEXTURE_2D, model->textures[IDTextura].id);		
		}

		if (mode & GLM_COLOR) {
			glColor3fv(material->diffuse);
		}

		glBegin(GL_TRIANGLES);
		for (i = 0; i < group->numtriangles; i++) {
			triangle = &T(group->triangles[i]);
#ifdef DebugVisibleSurfaces
			if (!triangle->visible) continue;
#endif
			if (mode & GLM_FLAT)
				glNormal3fv(&model->facetnorms[3 * triangle->findex]);

			if (mode & GLM_SMOOTH)
				glNormal3fv(&model->normals[3 * triangle->nindices[0]]);
			if (mode & GLM_TEXTURE)
				glTexCoord2fv(&model->texcoords[2 * triangle->tindices[0]]);
			glVertex3fv(&model->vertices[3 * triangle->vindices[0]]);

			if (mode & GLM_SMOOTH)
				glNormal3fv(&model->normals[3 * triangle->nindices[1]]);
			if (mode & GLM_TEXTURE)
			{
				//if (IDTextura==-1) printf("Warning: GLM_TEXTURE este on dar nu este setata nici o textura in material!");
				glTexCoord2fv(&model->texcoords[2 * triangle->tindices[1]]);
			}
			glVertex3fv(&model->vertices[3 * triangle->vindices[1]]);

			if (mode & GLM_SMOOTH)
				glNormal3fv(&model->normals[3 * triangle->nindices[2]]);
			if (mode & GLM_TEXTURE)
				glTexCoord2fv(&model->texcoords[2 * triangle->tindices[2]]);
			glVertex3fv(&model->vertices[3 * triangle->vindices[2]]);

		}
		glEnd();
		glPopMatrix();
		group = group->next;
	}
}

void init(void) 
{
	glEnable(GL_COLOR_MATERIAL);
	glClearColor (0.2, 0.6, 0.4, 0.0);
	//   glColor3f(1.0, 0.0, 0.0); 
	glEnable(GL_DEPTH_TEST);
	glShadeModel (GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	/*_displayListId_blackArea = glGenLists(1); //Make room for the display list

	  glNewList(_displayListId_blackArea, GL_COMPILE); //Begin the display list

	  draw_BlackArea(); //Add commands for drawing a black area to the display list

	  glEndList(); //End the display list



	//Set up a display list for drawing a cube

	_displayListId_whiteArea = glGenLists(2); //Make room for the display list

	glNewList(_displayListId_whiteArea, GL_COMPILE); //Begin the display list

	draw_whiteArea(); //Add commands for drawing a black to the display list

	glEndList(); //End the display list*/
}




void drawmodel_box(GLMmodel* pmodel)
{
	// Load the model only if it hasn't been loaded before
	// If it's been loaded then pmodel1 should be a pointer to the model geometry data...otherwise it's null
	//    if (!pmodel1) 
	//	{
	// this is the call that actualy reads the OBJ and creates the model object
	//      pmodel = glmReadOBJ("left_shoe.obj");
	//pmodel3 = glmReadOBJ("left_leg.obj");

	//      pmodel2 = glmReadOBJ("left_thigh.obj");	
	if (!pmodel) exit(0);
	//        if (!pmodel2) exit(0);
	//        if (!pmodel2) exit(0);
	// This will rescale the object to fit into the unity matrix
	// Depending on your project you might want to keep the original size and positions you had in 3DS Max or GMAX so you may have to comment this.
	glmUnitize(pmodel);
	//        glmUnitize(pmodel2);
	//        glmUnitize(pmodel3);
	// These 2 functions calculate triangle and vertex normals from the geometry data.
	// To be honest I had some problem with very complex models that didn't look to good because of how vertex normals were calculated
	// So if you can export these directly from you modeling tool do it and comment these line
	// 3DS Max can calculate these for you and GLM is perfectly capable of loading them
	glmFacetNormals(pmodel);       
	//        glmFacetNormals(pmodel2);   
	//        glmFacetNormals(pmodel3);
	glmVertexNormals(pmodel, 0.0);
	//		glmVertexNormals(pmodel2, 90.0);
	//		glmVertexNormals(pmodel3, 50.0);
	//  }
	// This is the call that will actualy draw the model
	// Don't forget to tell it if you want textures or not :))
	glmDraw1(pmodel, GLM_SMOOTH| GLM_TEXTURE,0);
	//    glmDraw(pmodel2, GLM_SMOOTH| GLM_TEXTURE);
	//	    glmDraw(pmodel3, GLM_SMOOTH| GLM_TEXTURE);
}
ground matrix[10][10];
float h=0.5;
int tx=0,tz=0;
int detect_collision(int val)
{
	int i,j;
	for(j=0;j<n_tiles;j++)
	{
		//	printf("dfsfs%f %f\n",matrix[0][j].x1,matrix[0][j].x2);
		if(txpos>matrix[0][j].x1 && txpos<matrix[0][j].x2)
		{
			break;
		}
	}
	for(i=0;i<n_tiles;i++)
	{
		//	printf("dewrwrewrew%f %f\n",matrix[i][0].z1,matrix[i][0].z2);
		if(tzpos>matrix[i][0].z1 && tzpos<matrix[i][0].z2)
		{
			break;
		}
	}
	tz=i;
	tx=j;
	/*	if(matrix[i][j].type==2)
		{
		matrix[i][j].height=h;
		}*/
	//	printf("%d %d\n",i,j);
	if(matrix[i][j].type==2 && val==1)
	{
		move=0;
	}
	else
		move=1;
	if(i==n_tiles-1 && j==n_tiles-1)
	{
		printf("YOU WON\n");
		sleep(2);
		exit(0);
	}
	if(i==n_tiles || j==n_tiles)
	{
		exit(0);
	}
	return matrix[i][j].type;
}
void drag(int x, int y){
	//y = 300.0 - y;
	x=250-x;
	y=400-y;
	hel_x=1.0*x/40;
	hel_z=1.0*y/40;
	//	printf("srsfsfsdfsdfsd%d %d\n",x,y);

	glutPostRedisplay();
}
void DrawCube(int i,int j)
{


	glPushMatrix();
	if(i==n_tiles-1 && j==n_tiles-1)
	{
		float a,b,c;
		a=1.0*(rand()%11)/10;
		b=1.0*(rand()%11)/10;
		c=1.0*(rand()%11)/10;
		glColor3f(a,b,c);

		glTranslatef(0,0,0);
		//		glScalef(0.2,0.2,0.2);
		glutSolidCube(2);
		glutWireCube(2);
	}
	else if (matrix[i][j].type==3)
	{
		glColor3f(0.0,0.7,0.7);

		glTranslatef(0,0,0);
		//		glScalef(0.2,0.2,0.2);
		glutSolidCube(2);
		glutWireCube(2);
	}
	else if (matrix[i][j].type==0)
	{
		glColor3f(0.0,0.4,0.7);

		glTranslatef(0,0,0);
		//		glScalef(0.2,0.2,0.2);
		glutSolidCube(2);
		glutWireCube(2);
	}
	else if (matrix[i][j].type==2)
	{
		glColor3f(0.0,0.6,0.2);

		//	glTranslatef(0,h/2,0);
		glScalef(1,h,1);
		glutSolidCube(2);
		glColor3f(1.0,0.0,0.0);
		glutWireCube(2);
	}
	else if (matrix[i][j].type==5)
	{
		float x=2.0,y=0.0;
		x=x-h;
		y=y+h;

		glColor3f(x,y,0.0);

		glTranslatef(0,0,0);
		//		glScalef(0.2,0.2,0.2);
		glutSolidCube(2);
		glutWireCube(2);
	}
	else if (matrix[i][j].type==4)
	{
		if(h>0.7 && h< 2.2)
		{
			glColor3f(0.0,0.6,0.2);
			float h1;
			//	glTranslatef(0,h/2,0);
			h1=h*2-1;
			glScalef(1,h1,1);
			glutSolidCube(2);
			glColor3f(1.0,0.0,0.0);
			glutWireCube(2);
			matrix[i][j].draw=1;
		}
		else
		{
			matrix[i][j].draw=0;
		}
	}

	//glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.0,1.0,1.0);
	glPopMatrix();
}
void DrawLine( int ax, int ay, int bx, int by, int width, int r, int g, int b)
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor3f( r, g, b);

	glLineWidth(width);
	glBegin(GL_LINES);
	glVertex2i( ax, ay);
	glVertex2i( bx, by);
	glEnd();

	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
}
void fall(void);
int camera=0;
int flag1=0,sel_i=1,sel_j=1,jump=0,dead=0;
void display(void)
{
	float ypoz1;
	ypoz1=ypoz*3.14/180;
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//	gluLookAt(10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glLoadIdentity ();
	GLfloat lightColor0[] = {0.6f, 0.6f, 0.6f, 1.0f};
	GLfloat lightPos0[] = {txpos-2.0, 2.0f, tzpos-2.0, 1.0f}; //positioning the light
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	if(camera==0)
	{

		gluLookAt(9.0,16.0,9.0,8.0,0.0,8.0, 0.0, 0.0 ,1.0);
	}
	if(camera==1)
	{
		//		gluLookAt(-3.0, 6.0,3.0, -4.0, 0.0, -5.0, 0.0, 1.0 ,0.0);
		gluLookAt(txpos-2.0*sin(ypoz1),typos+1.0  ,tzpos-2.0*cos(ypoz1),txpos/*+2.0*sin(ypoz1)*/,typos,tzpos/*+10.0*cos(ypoz1)*/ , 0.0, 1.0 ,0.0);
	}
	if(camera==2)
	{

		gluLookAt(txpos,typos+2,tzpos,txpos+10.0*sin(ypoz1),typos-2.0,tzpos+10.0*cos(ypoz1) , 0.0, 1.0 ,0.0);
	}
	if(camera==3)
	{

		gluLookAt(9.0,zoom,9.0,hel_x,0,hel_z, 0.0, 0.0 ,1.0);
		//gluLookAt(txpos,typos+2,tzpos-12,txpos+2.0*sin(ypoz1),typos-1.0,tzpos+-12+10.0*cos(ypoz1) , 0.0, 1.0 ,0.0);
	}
	if(camera==4)
	{
		float h_y;
		if(matrix[sel_i][sel_j].type==0)
		{
			h_y=0.5;
		}
		else
			h_y=h-1.0;

		gluLookAt(matrix[sel_i][sel_j].x1+1,h_y,matrix[sel_i][sel_j].z1+1,txpos,typos-1.0,tzpos , 0.0, 1.0 ,0.0);
	}
	GLfloat ambientLight[]={0.3f,0.3f,0.3f,1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLight);
	//glTranslatef(txpos,typos,tzpos+-12.0);
//	printf("before\n");
	for(int g=0;g<n_tiles;g++)
	  {
	  for(int h2=0;h2<n_tiles;h2++)
	  {
	  if(matrix[g][h2].type==2)
	  {
		  matrix[g][h2].height=(h-1.0);
//		  printf("look here %f %f\n",matrix[g][h2].height,h);
	  }
	  else if(matrix[g][h2].type==4)
	  {
		  matrix[g][h2].height=2.0*h-1.5;
//		  printf("look here %f %f\n",matrix[g][h2].height,h);
	  }
	  }
	  }
	if(((matrix[tz][tx].type==4 && matrix[tz][tx].draw==1 )|| matrix[tz][tx].type==2) && jump==0 )
	{
		typos=matrix[tz][tx].height;
	}
	else if(dead==0 && matrix[tz][tx].type==4 && matrix[tz][tx].draw==0)
	{
		dead=1;
		fall();
	}
	glPushMatrix();	
	//	glScalef(0.5,0.5,0.5);
	glTranslatef(txpos,typos+0.5,tzpos);
	glScalef(0.5,0.5,0.5);
	// I added these to be able to rotate the whole scene so you can see the box and textures
	glRotatef(ypoz,0,1,0);
	glRotatef(zpoz,0,0,1);	
	//	glTranslatef(txpos,typos,tzpos+-2.0);	
	if(!pmodel1)
	{
		pmodel1 = glmReadOBJ("man3.obj");
		//	pmodel2 = glmReadOBJ("terrain.obj");
	}
	//		pmodel3 = glmReadOBJ("ter.obj");
	drawmodel_box(pmodel1);
	glPopMatrix();
/*	glPushMatrix();
	glRotatef(90.0,1,0,0);
	DrawLine(-10.0,-2.0,-10.0,18.0,12.0,1.0,0.0,0.0);
	glPopMatrix();*/
	glPushMatrix();

	glTranslatef(0.0,-1.0,0.0);
	/*	glScalef(6.0,1.1,8.0);
	// I added these to be able to rotate the whole scene so you can see the box and textures
	glRotatef(0.0,0,1,0);
	glRotatef(0.0,0,0,1);
	//	glRotatef(30,1,0,0);
	drawmodel_box(pmodel2);
	//	drawmodel_box(pmodel3);
	glPopMatrix();*/
	if(flag1==0)
	{
		h=h+0.01;
	}
	else
		h=h-0.01;
	if(h>2.7)
		flag1=1;
	if(h<0.05)
		flag1=0;



	for(int ij=0;ij<n_tiles;ij++)
	{
		for(int ji=0;ji<n_tiles;ji++)
		{
			glPushMatrix();
			matrix[ij][ji].x1=2.0*ji-1;
			matrix[ij][ji].x2=2.0*ji+1;
			matrix[ij][ji].z1=2.0*ij-1;
			matrix[ij][ji].z2=2.0*ij+1;
			glTranslatef(2.0*ji,0,2.0*ij);
			DrawCube(ij,ji);
			glPopMatrix();
		}


	}
	glPopMatrix();
	/*
	   glMatrixMode(GL_MODELVIEW);

	   glLoadIdentity();
	   if(camera==1)
	   {
	   gluLookAt(-3.0, 6.0,3.0, -4.0, 0.0, -5.0, 0.0, 1.0 ,0.0);
	   }
	   if(camera==2)
	   {
	   float ypoz1;
	   ypoz1=ypoz*3.14/180;
	   gluLookAt(txpos,typos ,tzpos-12,txpos+2.0*sin(ypoz1),typos-1.0,tzpos-12+10.0*cos(ypoz1) , 0.0, 1.0 ,0.0);
	   }
	//glScalef(2.0,1.0,40.0);
	//	glPushMatrix();
	glRotatef(-_angle, 0.0f, 1.0f, 0.0f);

	glTranslatef(-4*1.5, -2.0, -5.0f);
	glRotatef(0,1,0,0);

	for(float j=0.0;j>(-8*1.5);j-=1.5)

	{

	k++;

	for(i=0.0;i<(4*3.0);i+=3.0)

	{

	if(k%2==0)

	{

	glPushMatrix();

	glTranslatef(i,0.0,j);

	glCallList(_displayListId_blackArea);

	glPopMatrix();



	}


	else

	{

	glPushMatrix();

	glTranslatef(i+1.5,0.0,j);

	glCallList(_displayListId_blackArea);

	glPopMatrix();



	}

	}

	}

	for(float j=0.0;j>(-8*1.5);j-=1.5)

	{

	k++;

	for(i=0.0;i<(4*3.0);i+=3.0)

	{

		if(k%2!=0)

		{

			glPushMatrix();

			glTranslatef(i,0.0,j);

			glCallList(_displayListId_whiteArea);

			glPopMatrix();



		}

		else

		{

			glPushMatrix();

			glTranslatef(i+1.5,0.0,j);

			glCallList(_displayListId_whiteArea);

			glPopMatrix();



		}

	}

}*/
//	glPopMatrix();

//glutSwapBuffers();








//Sleep(5);
glutSwapBuffers();


}
void fall(void)
{
	float temp_pos=typos,time=0.1;
	while(typos>-1.9)
	{
		typos=temp_pos-2*time*time;
		time=time+0.05;
		display();

	}
//	sleep(2);
	printf("GAME-OVER\n");
	exit(0);
}
void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
	glMatrixMode (GL_MODELVIEW);
}
float flag=1.0,vel=0.0,speed=1.0;
void keyboard2(int key, int x, int y)
{
	if(key==GLUT_KEY_DOWN || key==GLUT_KEY_UP)
	{
		vel=0.0;
		//		speed=1.0;
	}
}
void keyboard3(unsigned char key,int x,int y)
{
	if(key=='r')
	{
		speed=1.0;
	}
}
int col_prev=0;
void keyboard1(int key, int x, int y)
{
	float ypozr;
	ypozr=ypoz*3.14/180;
	if(frangle>20.0)
	{
		flag=-1.0*flag;
	}
	if(frangle<-20.0)
	{
		flag=-1.0*flag;
	}
	switch (key) {
		case GLUT_KEY_LEFT:         
			ypoz=ypoz+5;
			if (ypoz>360) ypoz=0;
			glutPostRedisplay();
			break;
		case GLUT_KEY_UP:
			vel=1.0;
			frangle=frangle+3.0*flag*speed;
			tzpos=tzpos+0.08*cos(ypozr)*speed;
			txpos=txpos+0.08*sin(ypozr)*speed;
			int col;
			col=detect_collision(1);
					if(col==1)
					{
						fall();
					}
			if(move==0 && col_prev==0 && h>1.0)
			{
				frangle=frangle-3.0*flag*speed;
				tzpos=tzpos-0.08*cos(ypozr)*speed;
				txpos=txpos-0.08*sin(ypozr)*speed;
				col=detect_collision(1);
			}
			col_prev=col;
			//printf("%f %f\n",tzpos,txpos);

			break;
		case GLUT_KEY_DOWN:
			vel=-1.0;
			speed=1.0;
			frangle=frangle-3.0*flag;
			//		tzpos=tzpos+0.08;
			tzpos=tzpos-0.08*cos(ypozr);
			txpos=txpos-0.08*sin(ypozr);
			int col1;
			if(col1==1)
			{
				fall();

			}
			col1=detect_collision(1);
			if(move==0 && col_prev==0 && h>1.0)
			{
				frangle=frangle+3.0*flag;
				//		tzpos=tzpos+0.08;
				tzpos=tzpos+0.08*cos(ypozr);
				txpos=txpos+0.08*sin(ypozr);
				col=detect_collision(1);
			}
			col_prev=col;
			break;
		case GLUT_KEY_RIGHT:
			ypoz=ypoz-5;
			if (ypoz<-360) ypoz=0;
			glutPostRedisplay();
			break;
	}

}
int t1x,t1z,t2x,t2z;
void keyboard(unsigned char key, int x, int y)
{
	float ypozr,typos1;
	ypozr=ypoz*3.14/180;
	switch (key) {
		case 27:
			exit(0);
			break;
		case 99:
			if(camera<4)
			{
				camera++;
			}
			else
			{
				camera=0;
			}
			break;
		case 't':
			int fi;
			fi=detect_collision(1);
			if(matrix[tz][tx].type==5)
			{
				if(tz==t1z && tx==t1x)
				{
					txpos=matrix[t2z][t2x].x1+1;
					tzpos=matrix[t2z][t2x].z1+1;

				}
				else
				{
					txpos=matrix[t1z][t1x].x1+1;
					tzpos=matrix[t1z][t1x].z1+1;

				}
			}
		case 'w':
			if(camera==4)
			{
				if(matrix[sel_i+1][sel_j].type!=1)
				{
					sel_i=sel_i+1;
				}
			}
			break;
		case 'a':
			if(camera==4)
			{
				if(matrix[sel_i][sel_j-1].type!=1)
				{
					sel_j=sel_j-1;
				}
			}
			break;
		case 's':
			if(camera==4)
			{
				if(matrix[sel_i-1][sel_j].type!=1)
				{
					sel_i=sel_i-1;
				}
			}
			break;
		case 'd':
			if(camera==4)
			{
				if(matrix[sel_i][sel_j+1].type!=1)
				{
					sel_j=sel_j+1;
				}
			}
			break;
		case '1':
			if(camera==3)
			{
				zoom=zoom+0.05;
			}
			break;
		case '2':
			if(camera==3)
			{
				zoom=zoom-0.05;
			}
			break;

		case 'r':
			speed=2.5;
			break;
		case ' ':
			float uy=4.0;
			float g=4.0;
			float temp,time=0.1;
			temp=frangle;
			frangle=60.0;
			jump=1;
			int typ=3;
			while(typ==3 && uy>1)
			{
				time=0.1;
			while(1)
			{
				typos1=uy*time-(g*time*time)/2;
				tzpos=tzpos+0.06*cos(ypozr)*vel*speed;
				txpos=txpos+0.06*sin(ypozr)*vel*speed;
				typos=typos1+typos;
				display();
				col_prev=detect_collision(0);
//				printf("%f %d %f\n",matrix[tz][tx].height,matrix[tz][tx].type,h-1);
				if(matrix[tz][tx].type==2 || matrix[tz][tx].type==4)
				{
					if(typos<h-1)
					{
						tzpos=tzpos-0.06*cos(ypozr)*vel*speed;
						txpos=txpos-0.06*sin(ypozr)*vel*speed;
						detect_collision(1);
						typos=typos-typos1;
						break;
					}
				}
				else if(matrix[tz][tx].height>typos1)
				{
					if(matrix[tz][tx].type!=1)
						typos=typos-typos1;
//					printf("%d %d %f %f\n",tx,tz,matrix[tx][tz].height,typos1);
					break;
				}
				typos=typos-typos1;

				time=time+0.05;
			}
			typ=col_prev;
			uy=uy-1.0;
			}
			jump=0;
			/*	 for(float time=0.1;time<2.0;time=time+0.1)
				 {
				 typos1=uy*time-(4.0*time*time)/2;
				 tzpos=tzpos+0.12*cos(ypozr)*vel*speed;
				 txpos=txpos+0.12*sin(ypozr)*vel*speed;
				 typos=typos1+typos;
				 display();
				 typos=typos-typos1;
				 }*/
			//	 col_prev=detect_collision(0);
			if(col_prev==1)
			{
				display();
				printf("GAME-OVER-JUMP\n");
				sleep(2);
				exit(0);
			}
			frangle=temp;
			break;
	}
}

void animate()
{
	//	ypoz+=0.5;
	//	tzpos=tzpos+0.01;
	if (ypoz>360) ypoz=0;
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init ();
	int count_tele=0,div=6;
	for(int i=0;i<n_tiles;i++)
	{
		for(int j=0;j<n_tiles;j++)
		{
			int a;
			a=rand()%div;
			if(a==5)
			{
				if(count_tele==0)
				{
					t1x=j;
					t1z=i;
				}
				else if(count_tele==1)
				{
					t2x=j;
					t2z=i;
				}
				if(i!=0 || j!=0)
				{
					count_tele++;
				}
			}
			if(count_tele==3)
			{
				j--;
				div--;
				count_tele++;
				continue;
			}
			matrix[i][j].type=a;
			if(a==0)
				matrix[i][j].height=0.0;
			else if(a==1) 
				matrix[i][j].height=-2.0;
			else
				matrix[i][j].height=0.0;
		}

	}
	matrix[0][0].type=0;
	matrix[0][0].height=0.0;
	matrix[7][7].type=0;
	matrix[7][7].height=0.0;
	glutDisplayFunc(display);
	// glutDisplayFunc(drawScene);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard1);
	glutSpecialUpFunc(keyboard2);
	glutKeyboardUpFunc(keyboard3);
	glutMotionFunc(drag);
	// glutTimerFunc(5, update, 0);
	// Do a little animation - rotate the object a bit so we can see it better
	glutIdleFunc(animate);
	glutMainLoop();
	return 0; 
}



#include "camera_simulator.h"


using namespace std;
using namespace Eigen;

void Init(int viewWidth, int viewHeight, double depthResolution, float* projmat = NULL);
void InitPers(int viewWidth, int viewHeight, double znear, double depthResolution, double* intrinsic, float* projmat=NULL);
void InitFE(int viewWidth, int viewHeight, double depthResolution, float* projMat=NULL);

void PanoramaRenderer::setData(float* vertex, unsigned int* face, float* reflectance, int vertNum, int meshnum) {
	if (dataNum == 0) {
		vertexPointers = (float**)malloc(sizeof(float*) * 1);
		facePointers = (unsigned int**)malloc(sizeof(float*) * 1);
		reflectancePointers = (float**)malloc(sizeof(float*) * 1);
		meshNumArray = (int*)malloc(sizeof(int) * 1);
		vtNumArray = (int*)malloc(sizeof(int) * 1);
		vertexPointers[0] = vertex;
		facePointers[0] = face;
		reflectancePointers[0] = reflectance;
		meshNumArray[0] = meshnum;
		vtNumArray[0] = vertNum;
		dataNum++;
	}
	else {
		dataNum++;
		vertexPointers = (float**)realloc(vertexPointers, sizeof(float*)*dataNum);
		facePointers = (unsigned int**)realloc(facePointers, sizeof(int*)*dataNum);
		reflectancePointers = (float**)realloc(reflectancePointers, sizeof(float*)*dataNum);
		meshNumArray = (int*)realloc(meshNumArray, sizeof(int)*dataNum);
		vtNumArray = (int*)realloc(vtNumArray, sizeof(int)*dataNum);
		vertexPointers[dataNum - 1] = vertex;
		facePointers[dataNum - 1] = face;
		reflectancePointers[dataNum - 1] = reflectance;
		meshNumArray[dataNum - 1] = meshnum;
		vtNumArray[dataNum - 1] = vertNum;
	};
}

void PanoramaRenderer::setDataRGB(float* vertex, unsigned int* face, unsigned char* rgba, int vertNum, int meshnum) {
	if (dataNum == 0) {
		vertexPointers = (float**)malloc(sizeof(float*) * 1);
		facePointers = (unsigned int**)malloc(sizeof(float*) * 1);
		rgbaPointers = (unsigned char**)malloc(sizeof(char*) * 1);
		meshNumArray = (int*)malloc(sizeof(int) * 1);
		vtNumArray = (int*)malloc(sizeof(int) * 1);
		vertexPointers[0] = vertex;
		facePointers[0] = face;
		rgbaPointers[0] = rgba;
		meshNumArray[0] = meshnum;
		vtNumArray[0] = vertNum;
		dataNum++;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertNum * 3 * sizeof(GLfloat), vertexPointers[0], GL_STATIC_DRAW);

		glGenBuffers(1, &cbo);
		glBindBuffer(GL_ARRAY_BUFFER, cbo);
		glBufferData(
			GL_ARRAY_BUFFER,
			3 * vertNum * sizeof(GLubyte),
			rgba,
			GL_STATIC_DRAW);

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			meshnum*3*sizeof(GLuint), // バッファのメモリ上のバイト数を求める
			face, //バッファへのポインタ
			GL_STATIC_DRAW
		);

		//glGenVertexArrays(1, &vao);
		//glBindVertexArray(vao);
		//glEnableVertexAttribArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}
	else {
		dataNum++;
		vertexPointers = (float**)realloc(vertexPointers, sizeof(float*)*dataNum);
		facePointers = (unsigned int**)realloc(facePointers, sizeof(int*)*dataNum);
		rgbaPointers = (unsigned char**)realloc(rgbaPointers, sizeof(char*)*dataNum);
		meshNumArray = (int*)realloc(meshNumArray, sizeof(int)*dataNum);
		vtNumArray = (int*)realloc(vtNumArray, sizeof(int)*dataNum);
		vertexPointers[dataNum - 1] = vertex;
		facePointers[dataNum - 1] = face;
		rgbaPointers[dataNum - 1] = rgba;
		meshNumArray[dataNum - 1] = meshnum;
		vtNumArray[dataNum - 1] = vertNum;
	};
}



void PanoramaRenderer::setDataRGB_(float* vertex, unsigned int* face, float* rgba, int vertNum, int meshnum) {

	meshNumArray = (int*)malloc(sizeof(int) * 1);
	vtNumArray = (int*)malloc(sizeof(int) * 1);
	meshNumArray[0] = meshnum;
	vtNumArray[0] = vertNum;
	glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertNum * 3 * sizeof(GLfloat),vertex, GL_STATIC_DRAW);

		glGenBuffers(1, &cbo);
		glBindBuffer(GL_ARRAY_BUFFER, cbo);
		glBufferData(
			GL_ARRAY_BUFFER,
			3 * vertNum * sizeof(GLfloat),
			rgba,
			GL_STATIC_DRAW);

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			meshnum * 3 * sizeof(GLuint), // バッファのメモリ上のバイト数を求める
			face, //バッファへのポインタ
			GL_STATIC_DRAW
		);
}


void PanoramaRenderer::setView() {
	if (type == PERSPECTIVE) {
		InitPers(viewWidth_, viewHeight_, znear, depthResolution, intrinsic, projMat);
	}
	else if (type == PANORAMA) {
		Init(viewWidth_, viewHeight_, depthResolution, projMat);
	}
	else if (type == FISHEYE) {
		InitFE(viewWidth_, viewHeight_, depthResolution, projMat);
	}
	else {
		std::cout << "The camera model is not selected!!" << std::endl;
		exit(-1);
	}
}

void PanoramaRenderer::rendering(Matrix4d& cameraParam) {
	GLint view[4];
	GLint maj_v, min_v;
	if (type == PERSPECTIVE) {
		glGetIntegerv(GL_VIEWPORT, view);
		perspectiveRendering(cameraParam);
	}
	else if(type == PANORAMA) {
		glGetIntegerv(GL_VIEWPORT, view);
		perspectiveRendering(cameraParam);
	}
	else if (type == FISHEYE) {
		glGetIntegerv(GL_VIEWPORT, view);
		perspectiveRendering(cameraParam);
	}
	else {
		std::cout << "The camera model is not selected!!" << std::endl;
		exit(-1);
	}
	//glfwSwapBuffers(win);

}

void PanoramaRenderer::memoryCopy() {
	GLint view[4];
	glGetIntegerv(GL_VIEWPORT, view);

	colorImage = (GLubyte*)malloc(sizeof(GLubyte)*view[2] * view[3] * 3);
	glReadPixels(view[0], view[1], view[2], view[3], GL_RGB, GL_UNSIGNED_BYTE, colorImage);
	depthArray = (GLfloat*)malloc(sizeof(GLfloat)*view[2] * view[3]);
	glReadPixels(view[0], view[1], view[2], view[3], GL_DEPTH_COMPONENT, GL_FLOAT, depthArray);

}

void PanoramaRenderer::perspectiveRendering(Matrix4d cameraParam) {
	//glClear(GL_DEPTH_BUFFER_BIT); // Depth bufferをクリアする
	glUseProgram(shaderProgram);
//	glBindVertexArray(vao);
	;
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(
		0,                  // シェーダ内のlayoutとあわせる
		3,                  // １要素の要素数(x,y,z)で3要素
		GL_FLOAT,           // タイプ
		GL_FALSE,           // 正規化しない（データが整数型の時）
		0,                  // ストライド
		(void*)0            // 配列バッファオフセット
	);

	// カラーバッファを有効にする
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glVertexAttribPointer(
		1,                  // シェーダ内のlayoutとあわせる
		3,                  // １要素の要素数(r,g,b)で3要素
		GL_FLOAT,           // タイプ
		GL_FALSE,           // 正規化しない（データが整数型の時）
		0,                  // ストライド
		(void*)0            // 配列バッファオフセット
	);
	GLdouble transMatd[16]; memcpy(transMatd, cameraParam.data(), sizeof(double) * 16);
	GLfloat transMat[16];
	for (int i = 0; i < 16; i++)transMat[i] = (float)transMatd[i];

	
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projMat"),1,GL_FALSE,projMat);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "w_position"), 1, GL_FALSE, transMat);
	if (type == FISHEYE) {
		for (int i = 0; i < 8; i++) {
			std::stringstream ss; ss << "lensParam[" + std::to_string(i) + "]";
			std::string uniLoc = ss.str();
			glUniform1f(glGetUniformLocation(shaderProgram, uniLoc.c_str()), lensParam[i]);
		}
	}


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//int size;
	//glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

	glDrawElements(
		GL_TRIANGLES,
		meshNumArray[0]*3,  // 頂点数。size / sizeof(GLuint) の式でも頂点数を算出できる。
		GL_UNSIGNED_INT,
		0
	);

	

}


void PanoramaRenderer::outputColor(string fileName) {
#if defined(_WIN32) || defined(_WIN64)
	ofstream	_os(fileName, ios::binary);
	if (!_os.is_open())
		return;
	//	Save image as bitmap.
	BITMAPFILEHEADER	_fh;
	BITMAPINFOHEADER	_if;
	int		_fh_size = sizeof(BITMAPFILEHEADER);
	int		_if_size = sizeof(BITMAPINFOHEADER);
	int		_size = viewHeight * viewHeight * 2 * 3;
	::ZeroMemory(&_fh, _fh_size);
	::ZeroMemory(&_if, _if_size);
	//	Initialize BITMAPFILEHEADER.
	_fh.bfSize = _fh_size + _if_size + _size;
	_fh.bfOffBits = _fh_size + _if_size;
	((char*)&_fh)[0] = 'B';
	((char*)&_fh)[1] = 'M';
	//	Initialize BITMAPINFOHEADER.
	_if.biSize = _if_size;
	_if.biWidth = viewHeight * 2;
	_if.biHeight = viewHeight;
	_if.biPlanes = 1;
	_if.biBitCount = 24;
	_if.biCompression = BI_RGB;
	_if.biSizeImage = _size;
	//	Reverse byte.
	//	unsigned char*		_buffer = new unsigned char[_size];
	unsigned char*		_buffer = (unsigned char*)malloc(sizeof(unsigned char)*_size);
	unsigned char*		_tdata = reinterpret_cast<unsigned char*>(colorImage);
	unsigned char*		_cdata = _buffer;
	int			i;
	for (i = 0; i < _size / 3; ++i) {
		//modification

		_cdata[0] = _tdata[2];
		_cdata[1] = _tdata[1];
		_cdata[2] = _tdata[0];
		_tdata += 3;
		_cdata += 3;
	}
	//	Write to file.
	_os.write((char*)&_fh, _fh_size);
	_os.write((char*)&_if, _if_size);
	_os.write(reinterpret_cast<char*>(_buffer), _size);
	_os.close();

	free(_buffer);
#endif
	return;

}

void PanoramaRenderer::outputReflectance(string fileName) {
	#if defined(_WIN32) || defined(_WIN64)
	ofstream	_os(fileName, ios::binary);
	if (!_os.is_open())
		return;
	//	Save image as bitmap.
	BITMAPFILEHEADER	_fh;
	BITMAPINFOHEADER	_if;
	int		_fh_size = sizeof(BITMAPFILEHEADER);
	int		_if_size = sizeof(BITMAPINFOHEADER);
	int		_size = viewHeight_ * viewWidth_ * 3;
	::ZeroMemory(&_fh, _fh_size);
	::ZeroMemory(&_if, _if_size);
	//	Initialize BITMAPFILEHEADER.
	_fh.bfSize = _fh_size + _if_size + _size;
	_fh.bfOffBits = _fh_size + _if_size;
	((char*)&_fh)[0] = 'B';
	((char*)&_fh)[1] = 'M';
	//	Initialize BITMAPINFOHEADER.
	_if.biSize = _if_size;
	_if.biWidth = viewWidth_;
	_if.biHeight = viewHeight_;
	_if.biPlanes = 1;
	_if.biBitCount = 24;
	_if.biCompression = BI_RGB;
	_if.biSizeImage = _size;
	//	Reverse byte.
	//	unsigned char*		_buffer = new unsigned char[_size];
	unsigned char*		_buffer = (unsigned char*)malloc(sizeof(unsigned char)*_size);
	float*		_tdata = reinterpret_cast<float*>(reflectanceImage);
	unsigned char*		_cdata = _buffer;
	int			i;
	for (i = 0; i < _size / 3; ++i) {
		//modification

		_cdata[0] = _tdata[0] * 255;
		_cdata[1] = _tdata[0] * 255;
		_cdata[2] = _tdata[0] * 255;
		_tdata += 1;
		_cdata += 3;
	}
	//	Write to file.
	_os.write((char*)&_fh, _fh_size);
	_os.write((char*)&_if, _if_size);
	_os.write(reinterpret_cast<char*>(_buffer), _size);
	_os.close();

	free(_buffer);
#endif
	return;

}


void PanoramaRenderer::outputDepth(string fileName) {
	#if defined(_WIN32) || defined(_WIN64)
	ofstream	_os(fileName, ios::binary);
	if (!_os.is_open())
		return;
	//	Save image as bitmap.
	BITMAPFILEHEADER	_fh;
	BITMAPINFOHEADER	_if;
	int		_fh_size = sizeof(BITMAPFILEHEADER);
	int		_if_size = sizeof(BITMAPINFOHEADER);
	int		_size = viewHeight_ * viewWidth_* 3;
	::ZeroMemory(&_fh, _fh_size);
	::ZeroMemory(&_if, _if_size);
	//	Initialize BITMAPFILEHEADER.
	_fh.bfSize = _fh_size + _if_size + _size;
	_fh.bfOffBits = _fh_size + _if_size;
	((char*)&_fh)[0] = 'B';
	((char*)&_fh)[1] = 'M';
	//	Initialize BITMAPINFOHEADER.
	_if.biSize = _if_size;
	_if.biWidth = viewWidth_;
	_if.biHeight = viewHeight_;
	_if.biPlanes = 1;
	_if.biBitCount = 24;
	_if.biCompression = BI_RGB;
	_if.biSizeImage = _size;
	//	Reverse byte.
	unsigned char*		_buffer = new unsigned char[_size];
	float*		_tdata = reinterpret_cast<float*>(depthArray);
	unsigned char*		_cdata = _buffer;
	int			i;
	for (i = 0; i < _size / 3; ++i) {
		unsigned char gr = _tdata[0] * 256 < 255 ? (unsigned char)(_tdata[0] * 256) : 255;

		_cdata[0] = gr;
		_cdata[1] = gr;
		_cdata[2] = gr;
				
		_tdata += 1;
		_cdata += 3;
	}
	//	Write to file.
	_os.write((char*)&_fh, _fh_size);
	_os.write((char*)&_if, _if_size);
	_os.write(reinterpret_cast<char*>(_buffer), _size);
	_os.close();

	delete[]	_buffer;
#endif
	return;



};

void PanoramaRenderer::outputNorm(string fileName) {
	#if defined(_WIN32) || defined(_WIN64)
	ofstream	_os(fileName, ios::binary);
	if (!_os.is_open())
		return;
	//	Save image as bitmap.
	BITMAPFILEHEADER	_fh;
	BITMAPINFOHEADER	_if;
	int		_fh_size = sizeof(BITMAPFILEHEADER);
	int		_if_size = sizeof(BITMAPINFOHEADER);
	int		_size = viewHeight * viewHeight * 2 * 3;
	::ZeroMemory(&_fh, _fh_size);
	::ZeroMemory(&_if, _if_size);
	//	Initialize BITMAPFILEHEADER.
	_fh.bfSize = _fh_size + _if_size + _size;
	_fh.bfOffBits = _fh_size + _if_size;
	((char*)&_fh)[0] = 'B';
	((char*)&_fh)[1] = 'M';
	//	Initialize BITMAPINFOHEADER.
	_if.biSize = _if_size;
	_if.biWidth = viewHeight * 2;
	_if.biHeight = viewHeight;
	_if.biPlanes = 1;
	_if.biBitCount = 24;
	_if.biCompression = BI_RGB;
	_if.biSizeImage = _size;
	float* normArray = (float*)malloc(sizeof(float)*viewHeight*viewHeight * 2);
	for (int x = 0; x < viewHeight * 2; x++) {
		for (int y = 0; y < viewHeight; y++) {
			int idx = y * viewHeight * 2 + x;
			if (y == 0 || y == viewHeight - 1) { normArray[idx] = 0; continue; }
			int prexidx = x != 0 ? y * viewHeight * 2 + x - 1 : y * viewHeight * 2 + viewHeight * 2 - 1;
			int postxidx = x != viewHeight * 2 - 1 ? y * viewHeight * 2 + x + 1 : y * viewHeight * 2;
			int preyidx = idx - viewHeight * 2;
			int postyidx = idx + viewHeight * 2;
			if (depthArray[idx] == 1.0 || depthArray[prexidx] == 1.0 || depthArray[postxidx] == 1.0 || depthArray[preyidx] == 1.0 || depthArray[postyidx] == 1.0) {
				normArray[idx] = 0;
				continue;
			}
			Vector3d n;
			double c = depthArray[idx] * M_PI * 4 / (viewHeight * 2);
			n << (depthArray[postxidx] - depthArray[prexidx]) / c, (depthArray[postyidx] - depthArray[preyidx]) / c, 1;
			n.normalize();

			normArray[idx] = n(2, 0);
		}
	}



	//	Reverse byte.
	unsigned char*		_buffer = new unsigned char[_size];
	float*		_tdata = reinterpret_cast<float*>(normArray);
	unsigned char*		_cdata = _buffer;
	int			i;
	for (i = 0; i < _size / 3; ++i) {



		unsigned char gr = _tdata[0] * 255 < 255 ? (unsigned char)(_tdata[0] * 255) : 255;

		_cdata[0] = gr;
		_cdata[1] = gr;
		_cdata[2] = gr;
		//		cout<<(int)_tdata[0]<<endl;
		_tdata += 1;
		_cdata += 3;
	}
	//	Write to file.
	_os.write((char*)&_fh, _fh_size);
	_os.write((char*)&_if, _if_size);
	_os.write(reinterpret_cast<char*>(_buffer), _size);
	_os.close();

	delete[]	_buffer;
#endif
	return;



};




void Init(int viewWidth,int viewHeight, double depthResolution, GLfloat* projMat) {
	//glViewport(0, 0, viewWidth, viewHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	//	  gluPerspective(90.0, (double)300/(double)300, 0.1, 100.0); //�������e�@�̎��̐�gluPerspactive(th, w/h, near, far);
	glOrtho(-PI_VAL, PI_VAL, -PI_VAL, 0, 0, depthResolution);
	//------------------------------------------------
	gluLookAt(
		0.0, 0.0, 0.0, // ���_�̈ʒux,y,z;
		0.0, 0.0, 1.0,   // ���E�̒��S�ʒu�̎Q�Ɠ_���Wx,y,z
		0.0, -1.0, 0.0);  //���E�̏�����̃x�N�g��x,y,z*/
	glGetFloatv(GL_PROJECTION_MATRIX, projMat);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void InitFE(int viewWidth, int viewHeight, double depthResolution, GLfloat* projMat) {
	//glViewport(0, 0, viewWidth, viewHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	//	  gluPerspective(90.0, (double)300/(double)300, 0.1, 100.0); //�������e�@�̎��̐�gluPerspactive(th, w/h, near, far);
	glOrtho(0, viewWidth,  -viewHeight,0 , 0.00, depthResolution);
	//------------------------------------------------
	gluLookAt(
		0, 0, 0.0, // ���_�̈ʒux,y,z;
		0,0 , 1.0,   // ���E�̒��S�ʒu�̎Q�Ɠ_���Wx,y,z
		0.0, -1.0, 0.0);  //���E�̏�����̃x�N�g��x,y,z*/
	glGetFloatv(GL_PROJECTION_MATRIX, projMat);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//    glFlush();
}


void InitPers(int viewWidth, int viewHeight,double znear ,double depthResolution, double* intrinsic, GLfloat* projMat) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);

	GLfloat m[16];

	Matrix4f m1_, r2l, rev;//projection
	double cx = intrinsic[0];
	double cy = intrinsic[1];
	double fx = intrinsic[2];
	double fy = intrinsic[3];
	double zfar = znear+depthResolution;
	//m1_ <<
	//	2 * fx / viewWidth, 0, (viewWidth - 2 * cx) / viewWidth, 0,
	//	0, -2 * fy / viewHeight, (viewHeight - 2 * cy) / viewHeight, 0,
	//	0, 0, -(zfar + znear) / (zfar - znear), - 2 * zfar*znear / (zfar - znear),
	//	0, 0, -1, 0;

	//m1_ <<
	//	2 * fx / viewWidth, 0, (viewWidth - 2 * cx) / viewWidth, 0,
	//	0, -2 * fy / viewHeight, (viewHeight - 2 * cy) / viewHeight, 0,
	//	0, 0, (zfar + znear) / (zfar - znear), 2 * zfar*znear / (zfar - znear),
	//	0, 0, -1, 0;

		//m1_ <<
		//2 * fx / viewWidth, 0, -(viewWidth - 2 * cx) / viewWidth, 0,
		//0, 2 * fy / viewHeight, -(viewHeight - 2 * cy) / viewHeight, 0,
		//0, 0, -(zfar + znear) / (zfar - znear), 2 * zfar*znear / (zfar - znear),
		//0, 0, 1, 0;

		m1_ <<
			2 * fx / viewWidth, 0, -(viewWidth - 2 * cx) / viewWidth, 0,
			0, 2 * fy / viewHeight, -(viewHeight - 2 * cy) / viewHeight, 0,
			0, 0, (zfar + znear) / (zfar - znear), -2 * zfar*znear / (zfar - znear),
			0, 0, 1, 0;

	Matrix4f m3 = m1_;

//	GLdouble m2[16];
	memcpy(projMat, m3.data(), sizeof(float) * 16);

//	glMultMatrixd(m2);
	//gluPerspective(90.0, viewWidth/(double)viewHeight, 0.1, 100.0);
	//gluLookAt(
	//	0.0, 0.0, 0.0,
	//	1.0, 0.0, 0.0,
	//	0.0, 1.0, 0.0);
}

//void PanoramaRenderer::createContext(int view_w,int view_h) {
//	viewWidth_ = viewWidth_stat = view_w;
//	viewHeight_ = viewHeight_stat = view_h;
//
//	if (::glfwInit() == GL_FALSE) {
//		std::cout << "glfwInit() error" << std::endl;
//		exit(0);
//	}
//	::glfwWindowHint(GLFW_VISIBLE, 0);
//	//::glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//	//::glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
//
//
//	win = ::glfwCreateWindow(view_w, view_h, "", NULL, NULL);
////	glfwSetWindowSizeLimits(win, 32, 32, GLFW_DONT_CARE, GLFW_DONT_CARE);
////	glfwSetWindowSize(win, view_w, view_h);
//	::glfwMakeContextCurrent(win);
//	std::cout << viewWidth_ << "," << viewHeight_ << std::endl;
//	::glfwGetWindowSize(win,&viewWidth_, &viewHeight_);
//	std::cout << viewWidth_ << "," << viewHeight_ <<std::endl;
//	std::cout << glGetString(GL_VERSION) << std::endl;;
//}
//
//void  PanoramaRenderer::discardContext() {
//	glfwTerminate();
//}




#if defined(_WIN32)


GLboolean PanoramaRenderer::createContext(int view_w, int view_h)
{
	viewWidth_ = viewWidth_stat = view_w;
	viewHeight_ = viewHeight_stat = view_h;
	if (false) {
		PIXELFORMATDESCRIPTOR _pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	//	Size of this struct
		1,	//	Versin of this structure
		PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL ,
		//		PFD_GENERIC_FORMAT,
				//	Pixel buffer flags
				PFD_TYPE_RGBA,	//	Type of pixel data
				24,	//	The number of color bitplanes
				0, 0, 0, 0, 0, 0,	//	Number of each color bitplanes and shift count
				0, 0,	//	Number of alpha bitplanes and shift count
				0, 0, 0, 0, 0,	//	Number of accumulation bits
				32,	//	Z depth
				0,	//	Stencil depth
				0,	//	Number of auxiliary buffers
				PFD_MAIN_PLANE,	//	Ignored
				0,	//	Reserved
				0,	//	Ignored
				0,	//	Transparent color value
				0,	//	Ignored
			};
			GLint view[4];
			HDC		_hdc_ = CreateCompatibleDC(NULL);
			//int viewWidth_ = view_w;
			//int viewHeight_ = view_h;
			DWORD m_DIBWidth = viewWidth_;
			DWORD m_DIBHeight = viewHeight_;
			DWORD m_BPP = 24;

			// Create BITMAPINFOHEADER
			BITMAPINFOHEADER* m_PBIH = new BITMAPINFOHEADER;
			int iSize = sizeof(BITMAPINFOHEADER);
			::memset(m_PBIH, 0, iSize);

			m_PBIH->biSize = sizeof(BITMAPINFOHEADER);
			m_PBIH->biWidth = m_DIBWidth;
			m_PBIH->biHeight = m_DIBHeight;
			m_PBIH->biPlanes = 1;
			m_PBIH->biBitCount = m_BPP;
			m_PBIH->biCompression = BI_RGB;

			// Create DIB
			void* m_PBits;
			HBITMAP m_hbitmap_old;
			HBITMAP m_hbitmap = ::CreateDIBSection(
				_hdc_,
				(BITMAPINFO*)m_PBIH, DIB_RGB_COLORS,
				&m_PBits, NULL, 0
			);

			m_hbitmap_old = (HBITMAP)::SelectObject(_hdc_, m_hbitmap);
			DWORD dwLength;
			if ((m_DIBWidth * 3) % 4 == 0) /* バッファの１ラインの長さを計算 */
				dwLength = m_DIBWidth * 3;
			else
				dwLength = m_DIBWidth * 3 + (4 - (m_DIBWidth * 3) % 4);

			int		_pfid = ChoosePixelFormat(_hdc_, &_pfd);
			::SetPixelFormat(_hdc_, _pfid, &_pfd);
			HGLRC	_hrc = ::wglCreateContext(_hdc_);
			::wglMakeCurrent(_hdc_, _hrc);
			return true;
	}
	else {
		WNDCLASS wc;
		PIXELFORMATDESCRIPTOR pfd;
		/* register window class */
		ZeroMemory(&wc, sizeof(WNDCLASS));
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpfnWndProc = DefWindowProc;
		wc.lpszClassName = "GLEW";
		if (0 == RegisterClass(&wc)) return GL_TRUE;
		/* create window */
		wnd = CreateWindow("GLEW", "GLEW", WS_POPUP, 0, 0, view_w,
			view_h, NULL, NULL, GetModuleHandle(NULL), NULL);



		if (NULL == wnd) return GL_TRUE;
		/* get the device context */
		dc = GetDC(wnd);
		if (NULL == dc) return GL_TRUE;
		/* find pixel format */
		ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
		//if (params->pixelformat == -1) /* find default */
		int pfid;
		{
			pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
			pfid = ChoosePixelFormat(dc, &pfd);
			if (pfid == 0) return GL_TRUE;
		}
		/* set the pixel format for the dc */
		if (FALSE == SetPixelFormat(dc, pfid, &pfd)) return GL_TRUE;
		/* create rendering context */
		rc = wglCreateContext(dc);
		if (NULL == rc) return GL_TRUE;
		if (FALSE == wglMakeCurrent(dc, rc)) return GL_TRUE;

		RECT lprc;
		GetWindowRect(wnd, &lprc);
		std::cout << lprc.bottom << "," << lprc.left << "," << lprc.top << "," << lprc.right << std::endl;

	}


	////if (params->major || params->profile || params->flags)
	////{
	////	HGLRC oldRC = rc;
	////	int contextAttrs[20];
	////	int i;

	////	wglewInit();

	////	/* Intel HD 3000 has WGL_ARB_create_context, but not WGL_ARB_create_context_profile */
	////	if (!wglewGetExtension("WGL_ARB_create_context"))
	////		return GL_TRUE;

	////	i = 0;
	////	if (params->major)
	////	{
	////		contextAttrs[i++] = WGL_CONTEXT_MAJOR_VERSION_ARB;
	////		contextAttrs[i++] = params->major;
	////		contextAttrs[i++] = WGL_CONTEXT_MINOR_VERSION_ARB;
	////		contextAttrs[i++] = params->minor;
	////	}
	////	if (params->profile)
	////	{
	////		contextAttrs[i++] = WGL_CONTEXT_PROFILE_MASK_ARB;
	////		contextAttrs[i++] = params->profile;
	////	}
	////	if (params->flags)
	////	{
	////		contextAttrs[i++] = WGL_CONTEXT_FLAGS_ARB;
	////		contextAttrs[i++] = params->flags;
	////	}
	////	contextAttrs[i++] = 0;
	////	rc = wglCreateContextAttribsARB(dc, 0, contextAttrs);

	////	if (NULL == rc) return GL_TRUE;
	////	if (!wglMakeCurrent(dc, rc)) return GL_TRUE;

	////	wglDeleteContext(oldRC);
	////}
	//return GL_FALSE;
}

void  PanoramaRenderer::discardContext() {
	if (NULL != rc) wglMakeCurrent(NULL, NULL);
	if (NULL != rc) wglDeleteContext(rc);
	if (NULL != wnd && NULL != dc) ReleaseDC(wnd, dc);
	if (NULL != wnd) DestroyWindow(wnd);
	UnregisterClass("GLEW", GetModuleHandle(NULL));
}

#endif


int PanoramaRenderer::viewWidth_stat;
int PanoramaRenderer::viewHeight_stat;

void PanoramaRenderer::setPersRender(double cx, double cy, double fx, double fy) {
	

	std::ifstream ifsvert("./opengl/pers.vert");
	std::string vsScript = std::string(istreambuf_iterator<char>(ifsvert),
		istreambuf_iterator<char>());
	std::cout << vsScript << std::endl;
	ifsvert.close();
	std::ifstream ifsfrag("./opengl/fragShader.frag");
	std::string fsScript = std::string(istreambuf_iterator<char>(ifsfrag),
		istreambuf_iterator<char>());
	std::cout << fsScript << std::endl;
	ifsfrag.close();

	persVertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vsScript_c= vsScript.c_str();
	glShaderSource(persVertexShader, 1,&vsScript_c , NULL);
	glCompileShader(persVertexShader);

	GLsizei loglen=2048, loglen_;
	GLchar infolog[2048];

	GLint compiled;
	glGetShaderiv(persVertexShader, GL_COMPILE_STATUS, &compiled);
	if (compiled != GL_TRUE) {
		glGetShaderInfoLog(persVertexShader, loglen, &loglen_, infolog);
		std::cout << "vertex shader error" << std::endl;
		std::cout << infolog << std::endl;
		exit(0);
	}


	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fsScript_c = fsScript.c_str();
	glShaderSource(fragShader, 1, &fsScript_c, NULL);
	glCompileShader(fragShader);

	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compiled);
	if (compiled != GL_TRUE) {
		glGetShaderInfoLog(fragShader, loglen, &loglen_, infolog);
		std::cout << "fragment shader error" << std::endl;
		std::cout << infolog << std::endl;
		exit(0);
	}



	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, persVertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);


	type = PERSPECTIVE;
	intrinsic[0] = cx;
	intrinsic[1] = cy;
	intrinsic[2] = fx;
	intrinsic[3] = fy;
};

void PanoramaRenderer::setEquirectangular() {


	std::ifstream ifsvert("./opengl/equi.vert");
	std::string vsScript = std::string(istreambuf_iterator<char>(ifsvert),
		istreambuf_iterator<char>());
	std::cout << vsScript << std::endl;
	ifsvert.close();
	std::ifstream ifsfrag("./opengl/fragShader.frag");
	std::string fsScript = std::string(istreambuf_iterator<char>(ifsfrag),
		istreambuf_iterator<char>());
	std::cout << fsScript << std::endl;
	ifsfrag.close();

	std::ifstream ifsgeom("./opengl/equi.geom");
	std::string gsScript = std::string(istreambuf_iterator<char>(ifsgeom),
		istreambuf_iterator<char>());
	std::cout << gsScript << std::endl;
	ifsgeom.close();

	persVertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vsScript_c = vsScript.c_str();
	glShaderSource(persVertexShader, 1, &vsScript_c, NULL);
	glCompileShader(persVertexShader);

	GLsizei loglen = 2048, loglen_;
	GLchar infolog[2048];

	GLint compiled;
	glGetShaderiv(persVertexShader, GL_COMPILE_STATUS, &compiled);
	if (compiled != GL_TRUE) {
		glGetShaderInfoLog(persVertexShader, loglen, &loglen_, infolog);
		std::cout << "vertex shader error" << std::endl;
		std::cout << infolog << std::endl;
		exit(0);
	}


	geomShader = glCreateShader(GL_GEOMETRY_SHADER);
	const char* gsScript_c = gsScript.c_str();
	glShaderSource(geomShader, 1, &gsScript_c, NULL);
	glCompileShader(geomShader);

	glGetShaderiv(geomShader, GL_COMPILE_STATUS, &compiled);
	if (compiled != GL_TRUE) {
		glGetShaderInfoLog(geomShader, loglen, &loglen_, infolog);
		std::cout << "geometry shader error" << std::endl;
		std::cout << infolog << std::endl;
		exit(0);
	}


	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fsScript_c = fsScript.c_str();
	glShaderSource(fragShader, 1, &fsScript_c, NULL);
	glCompileShader(fragShader);

	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compiled);
	if (compiled != GL_TRUE) {
		glGetShaderInfoLog(fragShader, loglen, &loglen_, infolog);
		std::cout << "fragment shader error" << std::endl;
		std::cout << infolog << std::endl;
		exit(0);
	}





	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, persVertexShader);
	glAttachShader(shaderProgram, geomShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);
	
	glGetProgramInfoLog(shaderProgram, loglen, &loglen_, infolog);
	std::cout << infolog << std::endl;


	type = PANORAMA;
};


void PanoramaRenderer::setFisheye(float* lensParam_) {


	std::ifstream ifsvert("./opengl/fe.vert");
	std::string vsScript = std::string(istreambuf_iterator<char>(ifsvert),
		istreambuf_iterator<char>());
	std::cout << vsScript << std::endl;
	ifsvert.close();
	std::ifstream ifsfrag("./opengl/fragShader.frag");
	std::string fsScript = std::string(istreambuf_iterator<char>(ifsfrag),
		istreambuf_iterator<char>());
	std::cout << fsScript << std::endl;
	ifsfrag.close();

	std::ifstream ifsgeom("./opengl/fe.geom");
	std::string gsScript = std::string(istreambuf_iterator<char>(ifsgeom),
		istreambuf_iterator<char>());
	std::cout << gsScript << std::endl;
	ifsgeom.close();

	persVertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vsScript_c = vsScript.c_str();
	glShaderSource(persVertexShader, 1, &vsScript_c, NULL);
	glCompileShader(persVertexShader);

	GLsizei loglen = 2048, loglen_;
	GLchar infolog[2048];

	GLint compiled;
	glGetShaderiv(persVertexShader, GL_COMPILE_STATUS, &compiled);
	if (compiled != GL_TRUE) {
		glGetShaderInfoLog(persVertexShader, loglen, &loglen_, infolog);
		std::cout << "vertex shader error" << std::endl;
		std::cout << infolog << std::endl;
		exit(0);
	}


	geomShader = glCreateShader(GL_GEOMETRY_SHADER);
	const char* gsScript_c = gsScript.c_str();
	glShaderSource(geomShader, 1, &gsScript_c, NULL);
	glCompileShader(geomShader);

	glGetShaderiv(geomShader, GL_COMPILE_STATUS, &compiled);
	if (compiled != GL_TRUE) {
		glGetShaderInfoLog(geomShader, loglen, &loglen_, infolog);
		std::cout << "geometry shader error" << std::endl;
		std::cout << infolog << std::endl;
		exit(0);
	}


	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fsScript_c = fsScript.c_str();
	glShaderSource(fragShader, 1, &fsScript_c, NULL);
	glCompileShader(fragShader);

	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compiled);
	if (compiled != GL_TRUE) {
		glGetShaderInfoLog(fragShader, loglen, &loglen_, infolog);
		std::cout << "fragment shader error" << std::endl;
		std::cout << infolog << std::endl;
		exit(0);
	}





	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, persVertexShader);
	glAttachShader(shaderProgram, geomShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	glGetProgramInfoLog(shaderProgram, loglen, &loglen_, infolog);
	std::cout << infolog << std::endl;

	type = FISHEYE;
	for (int i = 0; i < 8; i++)lensParam[i] = lensParam_[i];

};

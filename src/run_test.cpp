#include <camera_simulator.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

bool headString(std::string line, std::string chara) {
	if (line.find(chara) == 0)return true;
	else return false;
}

void ReadRGBPly(std::string input,float*& vtx, unsigned int*& face, float*& color, int& vtxnum, int& facenum) {
	std::ifstream ifs(input, std::ios::binary);
	std::string line;
	
	while (getline(ifs, line)) {
		std::cout << line << std::endl;
		//if (headString(line, "format")) {
		//	format = line.erase(0, 7);
		//}
		if (headString(line, "end_header"))break;

		if (headString(line, "element")) {
			line.erase(0, 8);
			if (headString(line, "vertex")) {
				line.erase(0, 7);
				vtxnum = stoi(line);
			}
			if (headString(line, "face")) {
				line.erase(0, 5);
				facenum = stoi(line);
			}
		}
	}

	vtx = (float*)malloc(sizeof(float)*vtxnum * 3);
	face = (unsigned int*)malloc(sizeof(unsigned int)*(facenum * 3));
	color = (float*)malloc(sizeof(float)*(vtxnum * 3));

	int n = 0;
	float f[5];
	unsigned char rgba[4];
	while (n < vtxnum && !ifs.eof()) {
		
		ifs.read((char *)f, sizeof(float)*4);
		vtx[n * 3] = f[0];
		vtx[n * 3 + 1] = f[1];
		vtx[n * 3 + 2] = f[2];
		ifs.read((char *)rgba, sizeof(unsigned char) * 4);
		color[n * 3] = rgba[0]/255.0;
		color[n * 3 + 1] = rgba[1] / 255.0;
		color[n * 3 + 2] = rgba[2] / 255.0;
		n++;
	}
	int facec = 0;
	while (facec < facenum && !ifs.eof()) {
		unsigned char i;
		int id1, id2, id3;
		ifs.read((char *)&i, sizeof(unsigned char));
		ifs.read((char *)&id1, sizeof(int));
		ifs.read((char *)&id2, sizeof(int));
		ifs.read((char *)&id3, sizeof(int));

		//	cout << i <<std::endl << id1 << id2 << id3 << std::endl;

		face[facec * 3] = id1;
		face[facec * 3 + 1] = id2;
		face[facec * 3 + 2] = id3;

		facec++;

		if (ifs.eof())
			id2 = 0;
	}
	
}









int main(int argc, char* argv[]) {

	std::string inputFile = "../model/model.ply";
	double cx = 400, cy = 320, f = 200, w = cx * 2, h = cy * 2;
	float lensParam[8];
	if(false){//perspective setting
		cx = 400;
		cy = 320;
		f = 200;
		w = cx * 2;
		h = cy * 2;
	}
	else if(true) {//panorama setting
		w = 4000;
		h = 2000;
	}
	else {
		w = 2400;
		h = 2400;
		lensParam[0] = 1200;
		lensParam[1] = 1200;
		lensParam[2] = 250;
		lensParam[3] = 250;
		lensParam[4] = 0;
		lensParam[5] = 0;
		lensParam[6] = 0;
		lensParam[7] = 0;
	}


	float* vtx;
	unsigned int* face;
	float* color;
	int vn, fn;
	ReadRGBPly(inputFile, vtx, face, color, vn, fn);

	//configure renderer, set verteces and meshes
	PanoramaRenderer pr;
	//create opengl context
	pr.createContext(w, h);
	if (glewInit() != GLEW_OK) {
		std::cout << "glewInit() error" << std::endl;
		return -1;
	};
	std::cout << glGetString(GL_VERSION) << std::endl;;
	GLint view[4];
	glGetIntegerv(GL_VIEWPORT, view);

	std::cout << "View :" << view[0] << "-" << view[1] << "-" << view[2] << "-" << view[3] << '\n';
	w = view[2];
	h = view[3];

	//setup camera view
	//pr.setPersRender(cx, cy, f, f); //for perspective 
	pr.setEquirectangular(); // for panorama
	//pr.setFisheye(lensParam);

	//add object
	pr.setDataRGB_(vtx, face, color, vn, fn); // pr.setData
	pr.setDepthFarClip(20.0);
	
	int viewHeight;
	int viewWidth;
	PanoramaRenderer::getViewSize(viewWidth, viewHeight);

	Matrix4d campos = Matrix4d::Identity();
	//rendereing with camera position
	pr.viewClear();
	pr.setView();
	pr.rendering(campos);
	pr.memoryCopy();

	uchar* dat = pr.getColorData();
	cv::Mat colorimage = cv::Mat(cv::Size(w, h), CV_8UC3);

	memcpy(colorimage.data, dat, sizeof(uchar) * 3 * colorimage.size().width*colorimage.size().height);
	cv::imwrite("color_visualize.png", colorimage);



	return 0;
}
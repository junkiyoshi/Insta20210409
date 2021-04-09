#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofNoFill();
	ofEnableDepthTest();
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_MULTIPLY);
	
	ofColor color;
	vector<int> hex_list = { 0xef476f, 0xffd166, 0x06d6a0, 0x118ab2, 0x073b4c };
	for (auto hex : hex_list) {

		color.setHex(hex);
		this->base_color_list.push_back(color);
	}

	for (int x = -300; x <= 300; x += 2) {

		this->base_location_list.push_back(glm::vec3(x, -300, -300));
		this->base_location_list.push_back(glm::vec3(x, -300, 300));
		this->base_location_list.push_back(glm::vec3(x, 300, -300));
		this->base_location_list.push_back(glm::vec3(x, 300, 300));
	}

	for (int y = -300; y <= 300; y += 2) {

		this->base_location_list.push_back(glm::vec3(-300, y, -300));
		this->base_location_list.push_back(glm::vec3(-300, y, 300));
		this->base_location_list.push_back(glm::vec3(300, y, -300));
		this->base_location_list.push_back(glm::vec3(300, y, 300));
	}

	for (int z = -300; z <= 300; z += 2) {

		this->base_location_list.push_back(glm::vec3(-300, -300, z));
		this->base_location_list.push_back(glm::vec3(-300, 300, z));
		this->base_location_list.push_back(glm::vec3(300, -300, z));
		this->base_location_list.push_back(glm::vec3(300, 300, z));
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	int radius = 3;
	while (this->log_list.size() < 3000) {

		int font_location_index = ofRandom(this->base_location_list.size());
		vector<glm::vec3> log;
		log.push_back(this->base_location_list[font_location_index]);
		this->log_list.push_back(log);
		this->color_list.push_back(this->base_color_list[(int)ofRandom(this->base_color_list.size())]);
		this->life_list.push_back(ofRandom(60, 120));
	}

	for (int i = this->log_list.size() - 1; i >= 0; i--) {

		this->life_list[i] -= 1;
		if (this->life_list[i] < 0) {

			this->log_list.erase(this->log_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
			this->life_list.erase(this->life_list.begin() + i);

			continue;
		}

		auto deg = ofMap(ofNoise(glm::vec4(this->log_list[i].back() * 0.0035, ofGetFrameNum() * 0.006)), 0, 1, -360, 360);
		auto theta = ofMap(ofNoise(glm::vec4(this->log_list[i].back() * 0.0035, (ofGetFrameNum() + 10000) * 0.006)), 0, 1, -360, 360);
		this->log_list[i].push_back(this->log_list[i].back() + glm::vec3(radius * cos(deg * DEG_TO_RAD) * sin(theta * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD) * sin(theta * DEG_TO_RAD), radius * cos(theta * DEG_TO_RAD)));
		while (this->log_list[i].size() > 100) {

			this->log_list[i].erase(this->log_list[i].begin());
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.25);

	for (int i = 0; i < this->log_list.size(); i++) {

		ofSetColor(this->color_list[i]);

		if (this->life_list[i] > 30) {

			ofSetLineWidth(1.5);
		}
		else {

			ofSetLineWidth(ofMap(this->life_list[i], 0, 30, 0, 1.5));
		}

		ofBeginShape();
		ofVertices(this->log_list[i]);
		ofEndShape();
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
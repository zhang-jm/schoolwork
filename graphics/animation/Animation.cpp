#include "Animation.h"

Animation::Animation(Skeleton * skel) {
	this->skel = skel;
}

Animation::~Animation() {

}

void Animation::parse(char * filename) {
	FILE* fp;
	char string[100];
	fpos_t pos;

	fopen_s(&fp, filename, "rb");
	if (fp == NULL) { std::cerr << "error loading file" << std::endl; exit(-1); }

	do {
		fscanf_s(fp, "%s", string);
		std::cout << string << std::endl;

		if (feof(fp)) {
			break;
		}

		if ((string[0] == 'r') && (string[1] == 'a')) //range
		{
			std::cout << "in range" << std::endl;

			float start, end;

			fscanf_s(fp, "%f %f", &start, &end);
			cout << start << " " << end << endl;

			startTime = start;
			endTime = end;
		}
		else if ((string[0] == 'n') && (string[1] == 'u')) //numchannels
		{
			std::cout << "in numchannels" << std::endl;

			float num;

			fscanf_s(fp, "%f", &num);
			cout << num << endl;

			numChannels = num;
		}
		else if ((string[0] == 'c') && (string[1] == 'h')) //channel
		{
			cout << "in channel" << endl;
			Channel * channel = new Channel();

			fscanf_s(fp, "%s", string);
			cout << string << endl;

			fscanf_s(fp, "%s", string);
			cout << string << endl;

			fscanf_s(fp, "%s", string);
			cout << string << endl;

			std::string exIn, exOut;
			fscanf_s(fp, "%s", string);
			exIn = string;
			fscanf_s(fp, "%s", string);
			exOut = string;
			cout << exIn << " " << exOut << endl;

			channel->extrapIn = exIn;
			channel->extrapOut = exOut;

			fscanf_s(fp, "%s", string);
			cout << string << endl;

			float numKeys;
			fscanf_s(fp, "%f", &numKeys);
			cout << numKeys << endl;

			fscanf_s(fp, "%s", string);
			cout << string << endl;

			for (int i = 0; i < numKeys; i++) {
				float time, range;
				std::string tanIn, tanOut;

				fscanf_s(fp, "%f", &time);
				cout << time << " ";
				fscanf_s(fp, "%f", &range);
				cout << range << " ";
				fscanf_s(fp, "%s", string);
				tanIn = string;
				fscanf_s(fp, "%s", string);
				tanOut = string;
				cout << tanIn << " " << tanOut << endl;

				channel->addKeyframe(time, range, tanIn, tanOut);
			}

			channels.push_back(channel);
			cout << "end of channel" << endl;
		}
	} while (!feof(fp));

	fclose(fp);
}

void Animation::evaluate(float time) {
	//cout << "animation evaluate" << endl;
	float xtrans, ytrans, ztrans;
	xtrans = channels.at(0)->evaluate(time);
	ytrans = channels.at(1)->evaluate(time);
	ztrans = channels.at(2)->evaluate(time);
	skel->joints.at(0)->setOffset(glm::vec3(xtrans, ytrans, ztrans));

	int channelNum = 3;
	//cout << "start" << endl;
	for (int i = 0; i < skel->joints.size(); i++) {
		float xrot, yrot, zrot;

		xrot = channels.at(channelNum)->evaluate(time);
		yrot = channels.at(channelNum + 1)->evaluate(time);
		zrot = channels.at(channelNum + 2)->evaluate(time);

		cout << skel->joints.at(i)->name << endl;
		//cout << xrot << " " << yrot << " " << zrot << endl;

		skel->joints.at(i)->setPose(glm::vec3(xrot, yrot, zrot));
		channelNum = channelNum + 3;
	}
}
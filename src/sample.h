#ifndef _H_SAMPLE_
#define _H_SAMPLE_

#include "Application.h"
#include "DebugDraw.h"
#include <vector>
#include "pose.h"
#include "clip.h"

class Sample : public Application {
protected:
	Pose mRestPose;
	Pose mCurrentPose;
	std::vector<Clip> mClips;
	unsigned int mCurrentClip;
	float mPlaybackTime;
	DebugDraw* mRestPoseVisual;
	DebugDraw* mCurrentPoseVisual;
public:
	void Initialize();
	void Update(float deltaTime);
	void Render(float inAspectRatio);
	void Shutdown();
};

#endif
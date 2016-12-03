#include "irrKlang/irrKlang.h"
#include <iostream>
#include <string>
#include <ctime>

using namespace irrklang;

class SoundEngine {
	public:
		SoundEngine() {
			// Start the sound engine
			engine = createIrrKlangDevice();
			// Error check that the sound engine is working
			if (!engine) {
				printf("Could not startup engine\n");
				exit(0); // error starting up the engine
			}
		}

		~SoundEngine() { engine->drop(); }

		void PlayLoop(const char* filename) {
			// play some sound stream, looped
			engine->play2D(filename, true);
		}

		void PlaySound(const char* filename) {
			engine->play2D(filename, false);
		}

	private:
		ISoundEngine* engine;
		bool soundPlaying;
};
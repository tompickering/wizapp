#include "scene.h"

#include <string>

#include "shared.h"

using std::string;

Scene::Scene(SceneRef ref) {
    scene_ref = ref;
    complete = false;
    time = 0.f;
    char_time = 0.1f;
    sentence = 0;
    sentence_time = 0.f;
    sentence_complete = false;
    switch (ref) {
        case Intro1:
            text.push_back(
                "Welcome to your final tests of wisdom and mind control, Apprentice.");
            text.push_back(
                "You are only a few steps away from becoming a real Wizard...");
            text.push_back(
                "But be assured, it won't be easy.");
            break;
        case Intro2:
            text.push_back(
                "During your quest, your mental abilities will be tested continually,");
            text.push_back(
                "as you will have to solve many different puzzles...");
            break;
        case Intro3:
            text.push_back(
                "Take this Magic Wand with you.");
            text.push_back(
                "It will be your indefatigable comrade, as it will");
            text.push_back(
                "help you to exploit all the mental powers that you have already obtained...");
            break;
        case Intro4:
            text.push_back(
                "Use it wisely, and you will have come very close to becoming a Wizard...");
            text.push_back(
                "Use it foolishly, and you will find yourself trapped in no time...");
            break;
        case Intro5:
            text.push_back(
                "Take this magic hat as well.");
            text.push_back(
                "It is the shield that will protect you physically");
            text.push_back(
                "during your long and tiresome quest...");
            break;
        case Intro6:
            text.push_back(
                "But bear in mind that your magical equipment are not enough in order");
            text.push_back(
                "to finish all the puzzles that you are about to confront...");
            break;
        case Intro7:
            text.push_back(
                "At the end of your quest, you will have proved yourself whether you are");
            text.push_back(
                "worthy of becoming a Wizard...");
            break;
        case Intro8:
            text.push_back(
                "May the Spirits of Courage, Strength and Wisdom");
            text.push_back(
                "follow you along your sacred mission...");
            break;
        case World1Start:
            text.push_back(
                "Welcome to the Great Valley!");
            text.push_back(
                "Your task here is to collect some flowers and mushrooms.");
            text.push_back(
                "Sounds easy? But don't be taken in, Great Valley is only a tutorial.");
            text.push_back(
                "May the force... ehh... good luck!");
            break;
        case World1Mid:
            text.push_back(
                "This is the Industrial Area.");
            text.push_back(
                "Complete its levels and you will have completed your basic training...");
            break;
        case World1End:
            text.push_back(
                "Congratulations Apprentice, your basic training is finally over.");
            text.push_back(
                "Now, you have to travel to the north, the SUN needs your help there...");
            break;
        case World2Start:
            text.push_back(
                "Greetings Apprentice!");
            text.push_back(
                "You know, this world used to be warm and full of life.");
            text.push_back(
                "Then... The Snowmaster came... He stole my energy and his");
            text.push_back(
                "Black Shadows covered the land, keeping it frozen.");
            text.push_back(
                "Please, help me gather my energy and remove the shadows.");
            text.push_back(
                "By the way... have you got a light?");
            break;
        case World2Mid:
            text.push_back(
                "HA! HA! HA!");
            text.push_back(
                "You are very bold to think that your intelligence can match my powers...");
            text.push_back(
                "Give it up and return home before I set my Shadows on you!");
            break;
        case World2End:
            text.push_back(
                "You did it, Apprentice!");
            text.push_back(
                "Thanks to you, this world can be once again a warm, happy place,");
            text.push_back(
                "where Summer never ends..!");
            break;
        case World3Start:
            text.push_back(
                "Yo wizard looking buzz check out my mess, and help me hit the jazz...");
            text.push_back(
                "My rusty notes are stolen by a high-tech brother,");
            text.push_back(
                "so you better hit the road, and help me get them back...");
            break;
        case World3Mid:
            text.push_back(
                "The notes are mine you Wizard brat!");
            text.push_back(
                "Press the 'Give Up' button now, you are only wasting your time!");
            break;
        case World3End:
            text.push_back(
                "Thanks to you bro, my voice is strong!");
            text.push_back(
                "I can sing again like Louis Armstrong!");
            text.push_back(
                "You did your job like a real super-hero");
            text.push_back(
                "and you opened the way to a whole new era..!");
            break;
        case World4Start:
            text.push_back(
                "Congratulations for getting so far, Apprentice, you took us by surprise...");
            text.push_back(
                "Things are getting rough for you now, since you have to travel deep in space");
            text.push_back(
                "to confront our sworn enemy, the Alieneer.");
            text.push_back(
                "Your mission is to collect the Power Stars that will allow us");
            text.push_back(
                "to fight him back...");
            break;
        case World4Mid:
            text.push_back(
                "Greetings conical humanoid... NOT!");
            text.push_back(
                "I am the Alieneer and I advise you to go back to your old men,");
            text.push_back(
                "before I get mad!");
            text.push_back(
                "Leave now... or suffer the consequences!");
            break;
        case World4End:
            text.push_back(
                "Bravo my child! We are all astonished by your performance!");
            text.push_back(
                "By collecting all the power stars, we managed to defeat him once and for all!");
            text.push_back(
                "The way to the FANTASIA land is now open for you..!");
            break;
        case World5Start:
            text.push_back(
                "Welcome to the FANTASIA world,");
            text.push_back(
                "where anything can happen!");
            text.push_back(
                "This is the final test of wisdom as you'll have to travel through");
            text.push_back(
                "the toughest puzzles and confront the fantasian chaos itself.");
            text.push_back(
                "Wish you luck... although luck has nothing to do with wisdom.");
            break;
        case World5Mid:
            text.push_back(
                "This is the end of your quest...");
            text.push_back(
                "The last test before reaching the level of WIZARD.");
            text.push_back(
                "This is the city of FANTASIA..!");
            break;
        case World5End:
            break;
        case Extro:
            break;
        case Friend:
            text.push_back(
                "Thank you so much, Apprentice, I owe you my life!");
            text.push_back(
                "I am obliged to you forever!");
            text.push_back(
                "Whenever you need me, I will be there to help you..!");
            break;
        case None:
            break;
        default:
            text.push_back("Foolish Tom has forgotten to add text for this scene!");
            text.push_back("Something something something dark side.");
            text.push_back("Something something something complete.");
    }
}

void Scene::update(float delta_time) {
    if (time == 0.f) {
        play_music();
    }

    time += delta_time;
    sentence_time += delta_time;
    if (scene_ref == World5End && time > 19)
        complete = true;
}

void Scene::play_music() {
    switch(scene_ref) {
        case Intro1:
        case Intro2:
        case Intro3:
        case Intro4:
        case Intro5:
        case Intro6:
        case Intro7:
        case Intro8:
            audio_manager.play_music("assets/audio/FAIRYTALE.ogg", false, true);
            break;
        case Friend:
            audio_manager.play_music("assets/audio/CUTY.ogg", false, true);
            break;
        case World5End:
            audio_manager.play_music("assets/audio/VLAKEIA.ogg", true, false);
            break;
        default:
            audio_manager.play_music("assets/audio/DIALOG.ogg", true, true);
            break;
    }
}

/* Should the level number be incremented following this scene? */
bool Scene::end_of_level() {
    switch(scene_ref) {
        case World1End:
        case World2End:
        case World3End:
        case World4End:
        case World5End:
            return true;
        default:
            return false;
    }
    return false;
}

string Scene::image_path() {
    switch(scene_ref) {
        case Intro1:
        case Intro2:
        case Intro7:
        case Intro8:
        case World1Start:
        case World1End:
        case World5Start:
        case World5End:
        case Extro:
            return "assets/img/intro/masters.png";
        case Intro3:
        case Intro4:
            return "assets/img/intro/nohat.png";
        case Intro5:
        case Intro6:
            return "assets/img/intro/hat.png";
        case World1Mid:
            return "assets/img/scene/world1_mid.png";
        case World2Start:
            return "assets/img/scene/world2_start.png";
        case World2Mid:
            return "assets/img/scene/world2_mid.png";
        case World2End:
            return "assets/img/scene/world2_end.png";
        case World3Start:
            return "assets/img/scene/world3_start.png";
        case World3Mid:
            return "assets/img/scene/world3_mid.png";
        case World3End:
            return "assets/img/scene/world3_end.png";
        case World4Start:
        case World4End:
            return "assets/img/scene/world4_start.png";
        case World4Mid:
            return "assets/img/scene/world4_mid.png";
        case World5Mid:
            return "assets/img/scene/world5_mid.png";
        case Friend:
            return "assets/img/scene/blobhi.png";
        default:
            return "assets/img/intro/masters.png";
    }
}

string Scene::current_text() {
    if (scene_ref == World5End) {
        return "";
    }
    if (sentence >= text.size())
        return "";
    unsigned int n_chars = (int) (sentence_time / char_time);
    if (n_chars > text.at(sentence).size() + 20) {
        sentence++;
        sentence_time = 0.f;
        if (sentence >= text.size()) {
            complete = true;
        }
        return "";
    }
    return text.at(sentence).substr(0, n_chars);
}

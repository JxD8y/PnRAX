#include <iostream>
#include "Ui/ImGui_Impl_easy.h"

void blankWindow1() {
    ImGui::Begin("Test Window 1");
    ImGui::Text("Hi im from Window 1");
    ImGui::End();
}
void blankWindow2() {
    ImGui::Begin("TestWWW");
    ImGui::Text("Hi im from Window 2");
    ImGui::End();
}
int main(int argc, char* argv[]) {
    Imeasy::AddRenderingObject(&blankWindow1);
    Imeasy::AddRenderingObject(&blankWindow2);
    Imeasy::StartRendering(
    "Test Window",800,600);
    return 0;
}
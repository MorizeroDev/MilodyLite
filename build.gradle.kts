plugins {
    id("party.para.h2cs") version "1.0.1" apply true
}

group = "com.morizero.milody"
version = "0.0.1"

subprojects {
    group = "com.morizero.milody"
    version = "0.0.1"
    repositories { repo() }
}

repositories { repo() }

tasks {
    h2cs {
        projectName = "Milody"
        sourceFilePath = System.getProperty("sourceFilePath")
            ?: throw Exception("No source files given")
        csharpBindingOutputPath = System.getProperty("csharpBindingOutputPath")
            ?: throw Exception("No csharp binding output path given")
        cppFrameworkBindingOutputPath = System.getProperty("cppFrameworkBindingOutputPath")
            ?: throw Exception("No cpp binding output path given")
        addTypeMapping(
            listOf("milody::audio::JuceAudioSourcePlayer") to "IntPtr",
            listOf("milody::audio::JuceAudioBuffer") to "IntPtr",
            listOf("milody::audio::JuceAudioFormatReader") to "IntPtr",
            listOf("milody::audio::JuceSoundPlayer") to "IntPtr",
            listOf("milody::audio::JuceRefAudioIODeviceCallback") to "IntPtr",
            listOf("milody::audio::JuceAudioDeviceManager") to "IntPtr",

            listOf("milody::context::Context") to "IntPtr",
        )
    }
}

fun RepositoryHandler.repo() {
    mavenLocal()
    mavenCentral()
    maven { url = uri("https://plugins.gradle.org/m2/") }
    maven { url = uri("https://maven.para.party") }
}

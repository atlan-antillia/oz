
<html>

<font size = 3 color="black">
<hr noshade color ="gray">

<b>

<b>
<font size=2>
1 Download OZ++<br>
</b>
</font>

<font size=2>
The latest OZ++ library for Linux and sample programs can be downloaded. 
</font>

</b>
<br>
<br>
<font size=2>
OZ++ Class Library 2.2.0 for Motif, OpenGL, OpenCV-4.2.0 and YOLOv3 on Ubuntu 18.04.2 (LTS)
<a href="http://www.antillia.com/oz++/oz++2.2.0.zip">OZ++ 2.2.0</a>

</font>)<br>
<br>
<!--
-->
</font>
<font color="black" size=2>

This library contains both a set of C++ class header files and a lot of sample programs of OZ++. <br>
</font>
<br>
<font size=2 color="black"><b>
In the latest oz++2.1.9, we have added a new sample <a href="http://www.antillia.com/oz++/yolov3/CustomYoloObjectDetector-CustomYoloObjectDetector.cpp.html">CustomYoloObjectDetector</a>
 for <a href="https://pjreddie.com/darknet/yolo/">YOLOv3</a>.
</b>
<br>
<br>
To use in the new sample, we have created <a href="http://www.antillia.com/oz++/classes/yolov3/Detector3.html"><b>Detector3</b></a> C++ class from <i>detector.c</i>, <i>image.c</i>, etc in darknet-master/src folder.
<br><br>
The <b>Detector3</b> class is written by our simple extension code to darknet-master/src, 
<a href="Javascript:confirmation('Are you sure you want to download darknet-master-to-use-std source?', 'http://www.antillia.com/oz++/darknet-master-to-use-std');"><b>darknet-master-to-use-std</b></a>
 to use with C++11 std::list and std::data classe.<br>
<br>
Furthermore, we have added <a href="http://www.antillia.com/oz++/classes/ConfigParser.html">ConfigParser<a> C++ class to parse an ini-file 
by using <a href="http://www.antillia.com/oz++/classes/KeyValueSplitter.html">KeyValueSplitter</a> 
and hash-map<a href="http://www.cplusplus.com/reference/unordered_map/unordered_map/">(std::unordered_map)</a>.
</font>
<br>
<br><br>
<font size=2>
<b>
2 What is OZ++? <br>
</b>
</font>

<font size=2>
<b>2.1 OZ++ 1.0</b>
<br>

&nbsp;OZ++ 1.0 is an experimental and elementary C++ class library for Linux. It is based on author's SOL9 C++ class library for Windows. OZ++ 1.0 has no GUI classes,.
We provide a set of C++ class header files and a lot of sample programs for those libaries.<br>
&nbsp;On OZ++ we implemented all the member functions inside the C++ classes. This is not a conventional coding style of C++ of separating implementation and interface. But OZ++ coding style has one point which enables a rapid prototyping of writing and testing C++ prograqms quickly. It may (or may not) be useful for various experimentation projects.
<br>
&nbsp;Historically, the author started to develop SOL9 C++ class library for Windows about a few years ago, based on SOL++2000 for Windows.

 The class hierarchy and class names of OZ++ are quite similar to those of SOL9. On OZ++ We have introduced a namespace 'OZ' to avoid the collisions of classes or type names in GNU g++ environment. 
<br>
 
&nbsp;We developed this OZ++ library by using g++ 7.3.0 on Ubuntu 18.04.2 LTS, but
 didn't test it on other g++ versions or Linux distributions.<br><br>
$ g++ --version<br>
g++ (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0<br>
Copyright (C) 2017 Free Software Foundation, Inc.<br>
<br>
In the latest OZ++ 2.1.8 libray, we have updated OZ::SmartPtr class to use std::unique_ptr of C++11.
<br><br>  
&nbsp;In OZ++ 1.0.5, we updated some experimental C++ classes(<font size=2 color="red">Netlink++ 1.0.3</font>) for Linux netlink, which seems to be a slightly complicated C library,
  but a very interesting material from an educational or practical point of view to study C++ class implementation for the library. Netlink++ may help you write programs to communicate with the Linux kernel based on the netlink.
<br>
&nbsp;In the version OZ++ 1.0.6, we updated some experimental C++ classes(<font size=2 color="red">Netlink++ 1.0.4</font>) for Linux netlink not to use OZ::String class.
<br>
<!--
<br>
&nbsp;In the version <font size=2 color="red">OZ++ 1.0.7</font> (2015/05/18), we added some experimental C++ classes for signal, inotify and fanotify of Linux.
<br>
&nbsp;In the version <font size=2 color="red">OZ++ 1.0.8</font> (2015/05/30), we added some experimental C++ classes for clock, epoll, eventfd, resource, signalfd, and timerfd of Linux.
<br>
&nbsp;In the version <font size=2 color="red">OZ++ 1.0.9</font> (2015/06/10), we updated C++ classes for epoll, inotify and fanotify of Linux.

<br>
&nbsp;In the version <font size=2 color="red">OZ++ 1.0.10</font> (2016/04/07), we updated C++ classes for glob, regex, wordex, sysinfo, sysconf, statvfs and statfs  of Linux.
<br>
&nbsp;In the latest version <font size=2 color="red">OZ++ 1.0.11</font> (2016/04/25), we have updated C++ classes for regex, socket, netdb of Linux.
</font>
<br>
 -->
<br>

<br>
<b>
<font size=2>
2.2 OZ++ 2.1<br>
2.2.1 OZ++ 2.1<br>
</b>

</font>

<font size=2>

&nbsp;OZ++ 2.1 is an extension C++ class library of OZ++ 2.0 to support the classic <a href="http://www.opengroup.org/openmotif/">Open Motif</a>-based GUI development 
environment on Ubuntu 18.04.2 (LTS).<br> 
The Motif C++ library (Motif++) is based on author's  old <a href="http://www.planetsourcecode.com/vb/scripts/BrowseCategoryOrSearchResults.asp?lngWId=3&blnAuthorSearch=TRUE&lngAuthorId=77422556&strAuthorName=Atlan&txtMaxNumberOfEntriesPerPage=25
">Oz++</a> library developed by AT&T C++ compiler on a UNIX system. (See also: Motif C++ Programming: Design and Implementation of Oz++; ISBN-13: 978-4833741019)<br>
&nbsp;The latest Open Motif version 2.3.8 has been released on December 2017 (<a href="https://sourceforge.net/projects/motif/">
Motif released as open source software under LGPL v2.1</a>).
If you would like to know Motif version on your system, it is helpfull to find XmVERSION_STRING in the file "/usr/include/Xm/Xm.h".
For example, you can see the following line:<br><br>
<font size=2 color="navy">
#define XmVERSION_STRING "@(#)Motif Version 2.3.8" <br>
</font>
<br>
&nbsp;In the latest OZ++2.1 library, we have been using Motif 2.3.8 to build our C++ GUI class library, which is used to write the GUI applications of
Motif, OpenGL and OpenCV.
<br>
</b>
<br>
 In the latest version OZ++2.1.7, we have updated CFLAGS to be empty in Makefiles in sample folders of xft to avoid an abormal termination in runtime.<br>
<br>
&nbsp;Motif++ is a set of thin wrapper C++ classes for Motif widgets. On Motif++ we implemented all the member functions inside the C++ classes, 
and introduced a namespace 'OZ'. 
<br>
<br>
<font size=2 color="red">
Schematically, you can imagine that:<br><br>
<b>
OZ++ 2.1 = (OZ++ 1.0) + (Motif++) + (OpenGL++) + (OpenCV++)
</b>
</font>
<br><br>  

We provide a set of C++ class header files and a lot of sample programs for this library.<br>
<br>

The OpenGL++ library of OZ++ 2.1 supports not only classic OpenGL APIs but also modern OpenGL Shader Language of <a href="http://glew.sourceforge.net/">GLEW</a> 
to avoid falling into chaos of devine OpenGL APIs, though we are not enthusiastic followers of OpenGL.<br><br>
The OpenCV++ library of OZ++ 2.1 supports the Motif-based GUI for the latest OpenCV-4.0.1 features instead of the standard HighGUI of OpenCV. <br>
In the latest OZ++, we have implemented simple thread classes OpenCVVideoReaderThread and OpenCVVideoWriterThread, which are derived from the OZ::Thread class.
Those classes will enable us to separate the event handlers for X11-Motif GUI events, and Video-Device I/O events.   
</font>


<br>
<br>
<font size=2>
<b>
2.2.2 Development environment of OZ++ 2.2<br>
</b>
</font>
<font size=2>

&nbsp; Although far behind others, we have upgraded our development environment of OZ++ from Ubuntu 16.04 LTS to Ubuntu 18.04.2 LTS (2019/04/6).<br>
After the installation the Ubuntu, we have added some development packages to the system in the following way, on the console.<br>
<br>
<table cellpadding="4"  width="98%" border=1 style='border-collapse: collapse;'>
<tr>
<td bgcolor='floralwhite'>
<font size=2>
# Update and upgrade<br> 
sudo apt-get update<br>
sudo apt-get upgrade<br>
<br>
# Update g++. We need the latest c++ compiler<br>
sudo apt-get install g++<br>
<br>
# Install X11, Xt libray<br>
sudo apt-get install libx11-dev<br>
sudo apt-get install libxt-dev<br>
<!-- sudo apt-get install libmotif-dev<br> -->
<br>
# Install OpenGL (Mesa) See : https://www.mesa3d.org/ and related library. <br>
# (See: <a href="http://www.omgubuntu.co.uk/2017/03/easy-way-install-mesa-17-0-2-ubuntu-16-04-lts">The Easy Way to Install Mesa 17.0.2 on Ubuntu 16.04 LTS</a>)<br>
sudo apt-get install mesa-common-dev<br>
sudo apt-get install freeglut3-dev<br>
sudo apt-get install libglew-dev<br>
<br> 
# Install xpm and jpeg library<br>
sudo apt-get install libxpm-dev<br>
sudo apt-get install libjpeg-dev<br>
<br>
# Install video for linux which can be used in OpenCV VideoCapture.<br>
sudo apt-get install libv4l-dev<br>

</font>
</td></tr>
</table>
<br>
<br>
<li>OpenCV-4.2.0</li>
We have built and installed the library for OpenCV-4.2.0 from the source code <a href = "https://github.com/opencv/opencv/releases">OpenCV-4.2.0</a>.
To use the legacy C constants and APIs in OpenCV-4.2.0, we have updated <a href="http://www.antillia.com/oz++/classes/OpenCVImageConverter.html">OpenCVImageConverter</a> class.
<br>
<br>
<li>OpenCV-4.1.0</li>
We have built and installed the library for OpenCV-4.1.0 from the source code <a href = "https://github.com/opencv/opencv/releases">OpenCV-4.1.0</a>.
To use the legacy C constants and APIs in OpenCV-4.1.0, we have updated <a href="http://www.antillia.com/oz++/classes/OpenCVObject.html">OpenCVObject</a> class and modified OZ++ sample programs. 
Furthermore, we have modified the Makefiles not to use pkg-config(
see <a href="https://github.com/opencv/opencv/issues/13154">4.0.0 does not produce pkgconfig file · Issue #13154 · opencv-opencv ·</a>).<br>
<br>

You may encounter a strange compilation error <b><i>undefined reference to std::__cxx11::basic_string</i></b>
to build a C++ OpenCV application as questioned in<br><br>
<a href="https://stackoverflow.com/questions/33394934/converting-std-cxx11string-to-stdstring">Converting std::__cxx11::string to std::string</a>
<br><br>
, and furthermore <br><br>
<b>
<a href="https://github.com/opencv/opencv/issues/13000">Undefined reference to cv::imread(std::string const&, int)</a><br>
<br>
</b>
As suggested in those web-sites, to fix these errors, you have to rebuild the OpenCV library by changing <i>CMakeCache.txt</i> file, in which the <b>CMAKE_CXX_FLAGS:STRING=</b>
 to have the following value, not empty as default.
<br><br>
//Flags used by the compiler during all build types.<br>
//CMAKE_CXX_FLAGS:STRING=<br>
<font size=2 color="red">
CMAKE_CXX_FLAGS:STRING=-D_GLIBCXX_USE_CXX11_ABI=0<br>
<br>
</font>
<font size=2>
Probably, you also have to add the following macro in your Makefile to build your application.<br><br> 
CFLAG = -D_GLIBCXX_USE_CXX11_ABI=0<br>
<br>
</font>
<br>
<font size=2>
<b>
2.2.3 History<br>
</b>
 Please see the page <a href="http://www.antillia.com/oz++/history.html">OZ++ Class Library History</a><br>
 
</font>

<br>
<table cellpadding="4"  width="98%" border=1 style='border-collapse: collapse;'>

<br>

<br>
<font size=2 color="black">
<b>2.3 OZ++ Features<br>
</font>
<font size=2 color="red">
<br>
 
 1. OZ++ is basically single inheritance oriented.<br>
 2. OZ++ classes have been implemented in C++ header files using inline member functions.<br>
 3. OZ++ applications only have to include OZ++ class header files, and they can be compiled without linking any OZ++ specific shared or static library.<br>
 4. OZ++ will support multiple charsets of ASCII(char) and UNICODE(wchar_t).<br>
 
</font>
<br>

<font size=2>
Note: Current OZ++ version is not thread safe. For example, the instances of LinkedList class cannot be safely accessed by multiple threads. </b>
</font>
<font size=2>
<br>
<br>
<font size=3>

<hr noshade color = "gray">
<b>
2 How to install OZ++ <br>
</b>
</b>
<font size=2 color="black">
You download the zip file oz++.zip, and unzip it on your working directory.program. For example, by unzipping on a directory ~/work, you get the following directories:
<br>
<!--
<pre>
-->
<br><b>
2.1 OZ++ 1.0
</b><br>
<b>~/work/oz++/iclude </b><br>
, which includes all C++ header files for OZ++ 1.0.<br>
<br>
<b>~/work/oz++/netlink</b><br>
, which includes some sample programs for netlink++ and build.sh shell script to compile them.
<br>

<br>
<b>~/work/oz++/samples</b><br>
, which includes all sample programs for OZ++ and build.sh shell script to compile them.
<br>
<br><b>
2.2 OZ++ 2.0
</b><br>
<b>~/work/oz++/include</b><br>
, which includes all C++ header files for OZ++ 1.0 and Motif++.<br>
<br>

<b>~/work/oz++/netlink</b><br>
, which includes some sample programs for netlink++ and build.sh shell script to compile them.
<br>
<br>
<b>~/work/oz++/samples</b><br>
, which includes all sample programs for OZ++ 1.0 and build.sh shell script to compile them.
<br>
<br>
<b>~/work/oz++/motif</b><br>
, which includes all sample programs for Motif++ and build.sh shell script to compile them.
<br>
<br>
<b>~/work/oz++/opengl</b><br>
, which includes all sample programs for OpenGL and build.sh shell script to compile them.
<br>
<br>
<b>~/work/oz++/opengl2</b><br>
, which includes all sample programs for OpenGL 2.0 and build.sh shell script to compile them.
<br>
<br>

<b>~/work/oz++/opencv</b><br>
, which includes some sample programs for OpenCV and build.sh shell script to compile them.
<br>

<br>
<b>~/work/oz++/openglcv</b><br>
, which includes some sample programs for OpenGL and OpenCV and build.sh shell script to compile them.
<br>

<br>
<br>
<b>~/work/oz++/xft</b><br>
, which includes some sample programs for XFT and build.sh shell script to compile them.
<br>

<br>

<font size=3>
<hr noshade color = "gray">
<b>
3 How to write OZ++ applications.
</font><br>
</b>

<font size=2>
&nbsp;We show some simple and elementary application programs of OZ++, Netlink++, Motif++, OpenCV and OpenGL which may be helpful to understand 
OZ++ programming style. For more examples, please see <a href="http://www.antillia.com/oz++/samples.html">OZ++ samples</a><br> <br>
</font>
<br>
<a href="#3.1.1">3.1.1 Hello world</a><br><br>

<a href="#3.2.1">3.2.1 InterfaceAddrRequest</a><br><br>

<a href="#3.2.2">3.2.2 RoutingTableNewRequest</a><br><br>

<a href="#3.2.3">3.2.3 IPAddressMonitor</a><br><br>

<a href="#3.3.1">3.3.1 Label</a><br><br>

<a href="#3.3.2">3.3.2 Pixelmap and EventHandler</a><br><br>

<a href="#3.3.3">3.3.3 ApplicationView and Callback</a><br><br>

<a href="#3.3.4">3.3.4 TreeView</a><br><br>

<a href="#3.3.5">3.3.5 XFT</a><br><br>

<a href="#3.3.5.1">3.3.5.1 CustomScrolledView</a><br><br>

<a href="#3.3.6">3.3.6 Is Motif a sleeping beauty?</a><br><br>

<a href="#3.3.7">3.3.7 FTFontPreview</a><br><br>

<a href="#3.3.8">3.3.8_FileListView</a><br><br>

<a href="#3.3.9">3.3.9 FileIconBox</a><br><br>

<a href="#3.3.10">3.3.10 IconBar</a><br><br>

<a href="#3.3.11">3.3.11 IPAddressWatcher</a><br><br>

<a href="#3.3.12">3.3.12 Polygon</a><br><br>

<a href="#3.3.13">3.3.13 ImageViewer</a><br><br>

<a href="#3.3.14">3.3.14 Texture</a><br><br>

<a href="#3.3.15">3.3.15 InodeWatcher</a><br><br>

<a href="#3.3.16">3.3.16 LightedTorus</a><br><br>

<a href="#3.3.17">3.3.17 MultiJPGTexturedCubeRotationByTimerThread</a><br><br>

<a href="#3.3.18">3.3.18 MultiColoredTrianglShader</a><br><br>

<a href="#3.3.19">3.3.19 BufferedColoredRegularIcosahedron</a><br><br>

<a href="#3.3.20">3.3.20 SphereWithColorEyeLightPositioner</a><br><br>

<a href="#3.3.21">3.3.21 MaterializedTorusesRotationByTimerThread</a><br><br>

<a href="#3.3.22">3.3.22 TexturedSphereRotationByKeyInput</a><br><br>

<a href="#3.3.23">3.3.23 StarSystemModel</a><br><br>

<a href="#3.3.24">3.3.24 FileSystemBrowser</a><br><br>

<a href="#3.3.25">3.3.25 ImageBlur</a><br><br>

<a href="#3.3.26">3.3.26 ImageSharpen</a><br><br>

<a href="#3.3.27">3.3.27 ObjectDetector</a><br><br>

<a href="#3.3.28">3.3.28 VideoDeviceEnumerator</a><br><br>

<a href="#3.3.29">3.3.29 OpenGLCVImageViews</a><br><br>

<a href="#3.3.30">3.3.30 BlurredMaterializedSpheres</a><br><br>

<a href="#3.3.31">3.3.31 ImageTransformationByDynamicColorFilter</a><br><br>

<a href="#3.3.32">3.3.32 TexturedSphereWithAxisEyeAndLightPositioner</a><br><br>

<a href="#3.3.33">3.3.33 CustomYoloObjectDetector</a><br><br>

<br>

<font size=2>3.1 OZ++ 1.0</font>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.1.1"><a href="http://www.antillia.com/oz++/3.1.1_HelloWorld.html">1.1 Hello world</a></a><br>
 On OZ++ application you simply include OZ++ class header files as your needs, as shown below. You also have to write your own main function of the C/C++ program entry point. 
 </font>
 <br><br>
<a href="http://www.antillia.com/oz++/3.1.1_HelloWorld.html"><img src= "http://www.antillia.com/oz++/images/HelloWorld.png" ></a>
<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.1.2"><a href="http://www.antillia.com/oz++/3.1.2_ThreadAndMessageQueue.html"><br>3.1.2 Thread and MessageQueue</a><br>
&nbsp;This is an example to write data to POSIX message queue by using OZ++ Thread class.<br>
</font>
<font size=2 color="red">
Note:<br>
1. Define your own C++ class (Writer) as a subclass of Thread class.<br>
2. Add a member variable of MessageQueue class which can be used to send data to other process.<br>
3. Define a contructor to initialize the above member variable.<br>
4. Define a run method to send your data to other process through MessageQueue.<br>
5. In main function, create an instance of your thread class(Writer), and call start and wait methods for the instance.<br>

</font>
<br>
<a href="http://www.antillia.com/oz++/3.1.2_ThreadAndMessageQueue.html"><img src= "http://www.antillia.com/oz++/images/MessageQueue.png" ></a>>
<br>
<br>
<font size=2>3.2 Netlink++</font><br>

<font size=2>&nbsp;&nbsp;<a name="3.2.1"><a href="http://www.antillia.com/oz++/3.2.1_InterfaceAddrRequest.html">3.2.1 InterfaceAddrRequest</a></a><br>
&nbsp;On Netlink++ application you simply include OZ++ netlink class header files as your needs, as shown below. 
You also have to write your own main function of the C/C++ program entry point. <br>
This is an example to send a NetlinkRequest for the structure ifaddrmsg to get IPv4 interface addresses.<br>
</font>
<font size=2 color="red">
Note:<br>
1. Define your own netlink request C++ class.<br>
2. Add a member variable of NetlinkRequest&lt;T&gt; which includes a netlink message header and a requst body of type T.<br>
3. Add a member variable of SocketNetlink which can be used as a communication channel to the kernel.<br>
4. Define a contructor to initialize the above two member variables.<br>
5. Define a send method to send your netlink request to the kernel through the SocketNetlink.<br>
6. Define a recv method to receive a reply from the kernel through the SocketNetlink.<br>
7. In main function, create an instance of your request class, and call the send and recv mothods for the instance.<br>

</font></td></tr>
<a href="http://www.antillia.com/oz++/3.2.1_InterfaceAddrRequest.html"><img src= "http://www.antillia.com/oz++/images/InterfaceAddrRequest.png" ></a>
<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.2.2"><a href="http://www.antillia.com/oz++/3.2.2_RoutingTableNewRequest.html">3.2.2 RoutingTableNewRequest</a></a><br>
This is an example to send a NetlinkRequest for the structure rtmsg to register a new route.<br>
</font>
<font size=2 color="red">
Note:<br>
1. Define your own netlink request C++ class.<br>
2. Add a member variable of NetlinkRouteAttributeRequest&lt;T&gt; which includes a netlink message header and a requst body of type T.<br>
3. Add a member variable of SocketNetlink which can be used as a communication channel to the kernel.<br>
4. Add a member variable of NetlinkReply&lt;T&gt; which includes a netlink message header and ack data from the kernel.<br>
5. Define a contructor to initialize the above two member variables.<br>
6. Define a send method to send your netlink request to the kernel through the SocketNetlink.<br>
7. Define a recv method to receive a reply from the kernel through the SocketNetlink.<br>
8. In main function, create an instance of your request class, and call the send and recv mothods for the instance.<br>
<br>
</font>
<a href="http://www.antillia.com/oz++/3.2.2_RoutingTableNewRequest.html"><img src= "http://www.antillia.com/oz++/images/RoutingTableNewRequest.png" ></a>
<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.2.3"><a href="http://www.antillia.com/oz++/3.2.3_IPAddressMonitor.html">3.2.3 IPAddressMonitor</a></a><br>
This is an example to monitor changes of InterfaceAddresses for IPv4 and IPv6.
</font><br>
<font size=2 color="red">
Note:<br>
1. Define your own netlink monitor C++ class.<br>
2. Add a member variable for groups to monitor.<br>
3. Add a member variable of SocketNetlink which can be used as a communication channel to the kernel.<br>
4. Add a member variable of NetlinkReply&lt;T&gt; which includes a netlink message header and ack data from the kernel.<br>
5. Define a contructor to initialize the group member variable.<br>
6. Define a bind method to communicate with the kernel through the SocketNetlink.<br>
7. Define a recv method to receive a notification from the kernel through the SocketNetlink.<br>
8. In main function, create an instance of your monitor class, and call bind and recv mothods for the instance.<br>

</font>
<a href="http://www.antillia.com/oz++/3.2.3_IPAddressMonitor.html"><img src= "http://www.antillia.com/oz++/images/IPAddressMonitor.png" ></a>
<br>
<br>
<font size=2>3.3 Motif++, Xft++, and OpenGL++</font>
<br>

<font size=2>&nbsp;&nbsp;<a name="3.3.1"><a href="http://www.antillia.com/oz++/3.3.1_Label.html">3.3.1 Label</a></a><br>
&nbsp;On Motif++ application for OZ++ 2.0, you simply include Motif++ class header files for your requirement, as shown below. <br>
<font size=2 color="red">
Note:<br>
1. Define your own top level application view class(MainView) to be a subclass of OZ::ApplicationView class. <br>
2. In main function, create an instance of OZ::Application class, and
an instance of your own application view class(MainView). <br>
3. Call realize method of the instance of the application view.<br>
4. Call run method of the instance of Application to enter an event loop.
<br>
</font>

</font>
<br>
<br>
<a href="http://www.antillia.com/oz++/3.3.1_Label.html"><img src= "http://www.antillia.com/oz++/images/Label.png" width=360 ></a>
<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.3.2"><a href="http://www.antillia.com/oz++/3.3.2_PixelmapAndEventHandler.html">3.3.2 Pixelmap and EventHandler</a></a><br>
&nbsp;Tis is an example to create a pixelmap and draw colored points on it, and copy the pixelmap
onto a window by an eventhandler for ExposureMask. <br>
<font size=2 color="red">
Note:<br>
1. Define your own top level application view class(MainView) to be a subclass of OZ::ApplicationView class. <br>
2. Define a member varialbe for Pixelmap class to store colored points.<br>
3. Define a member variable for DC class which can be used as a device(graphics) context for the pixelmap.<br> 
4. Define a constructor to intialize the above member variables.<br>
5. Define a draw eventhandler method to copy the pixelmap to your window. <br>
6. In main funcition, create create an instance of OZ::Application class, and an instance of your own application view class(MainView). <br>
7. Call realize method of the instance of the application view.<br>
8. Call run method of the instance of Application to enter an event loop.
<br>
<br>
</font>
<a href="http://www.antillia.com/oz++/3.3.2_PixelmapAndEventHandler.html"><img src= "http://www.antillia.com/oz++/images/ColorModel.png" ></a>
<br>
<br>

<font size=2>&nbsp;&nbsp;<a name="3.3.3"><a href="http://www.antillia.com/oz++/3.3.3_ApplicationViewAndCallback.html">3.3.3 ApplicationView and Callback</a></a><br>
&nbsp;On Motif++ application for OZ++ 2.0, you can write easily a typical application window with a menubar and pulldown menus, as shown below. <br>
This is an example to select a text file from a fileDialog, and to display the content of the selected file in a scrolledText window.<br>

<font size=2 color="red">
Note:<br>
1. Define your own top level application view class(MainView) to be a subclass of OZ::ApplicationView class. <br>
2. Define member vairlables for MainWindow, MenuBar, PulldownMenu, ScrolledTextm, and  FileDialog.<br>
3. Define a constructor to create the instances of the above classes, and register callbacks to a menu and a file selection.<br>
4. Define calllback method(fileMenu) for a menu selection, and callback method (open) for a file selection in a fileDialog.<br>
5. In main function, create an instance of your OZ::Application class, and
an instance of your own application view class(MainView). <br>
6. Call realize method of the instance of the application view.<br>
7. Call run method of the instance of Application to enter an event loop.<br>
</font><br>
<a href="http://www.antillia.com/oz++/3.3.3_ApplicationViewAndCallback.html"><img src= "http://www.antillia.com/oz++/images/ApplicationView.png" ></a>
<br>
<br>

<font size=2>&nbsp;&nbsp;<a name="3.3.4"><a href="http://www.antillia.com/oz++/3.3.4_TreeView.html">3.3.4 TreeView</a></a><br>
&nbsp;On Motif++, you can write a TreeView application based on xmTreeWidgetClass (Open Motif 2.2 and later) as shown below.<br>
This is an example to display a tree of some subnodes of PushButtons or Labels.<br>

<font size=2 color="red">
Note:<br>
1. Define your own top level application view class(MainView) to be a subclass of OZ::ApplicationView class. <br>
2. Define a member vairlable for Tree class.<br>
3. Define a constructor to create the instances of Tree, and add nodes (PushButtons or Labels) to the tree instance.<br>
4. Define calllback method(pushed) for the PushButtons on the nodes.<br>
5. In main function, create an instance of your OZ::Application class, and
an instance of your own application view class(MainView). <br>
6. Call realize method of the instance of the application view.<br>
7. Call run method of the instance of Application to enter an event loop.<br>

</font></td></tr>
<a href="http://www.antillia.com/oz++/3.3.4_TreeView.html"><img src= "http://www.antillia.com/oz++/images/Tree.png"></a>
<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.3.5"><a href="http://www.antillia.com/oz++/3.3.5_XFT.html">3.3.5 XFT</a></a><br>
&nbsp;As you know, Open Motif 2.3 (<a href="http://www.google.co.jp/url?sa=t&rct=j&q=&esrc=s&source=web&cd=8&ved=0CFIQFjAH&url=http%3A%2F%2Ffossies.org%2Flinux%2Fmisc%2Fold%2Fopenmotif-2.3.0-manual.pdf.tgz%2Fappendixg.pdf&ei=HrvGVLSYAcG2mAXvyIHACw&usg=AFQjCNFUbt_IJKZoqVcp7xnRgOezSu-VLg&sig2=qZlEJtcl7lkz27cw6aC0RA&bvm=bv.84349003,d.dGY">Appendix G - New Features in Motif 2.3</a>) and later support the free type fonts Xft
(X free type). <br>
To use XFT fonts on Motif, you only have to write some XmRenderTable specific resources into .Xdefaults file as shown below.<br>
<br>
<img src="http://www.antillia.com/oz++/images/Xdefaults.png"><br>
<br>
Please note that the *fontTye is a mandatory item and must take <font size=2 color="red">FONT_IS_XFT</font> for Xft fonts.<br>
 Fortunately, you don't need to change your already-written C/C++ source files for using Xft.

</font>
<br>
<br>
<a href="http://www.antillia.com/oz++/3.3.5_XFT.html"><img src= "http://www.antillia.com/oz++/images/DirectoryScanner.png" ></a>
<br>
<br>

<font size=2>&nbsp;&nbsp;<a name="3.3.5.1"><a href="http://www.antillia.com/oz++/3.3.5.1_CustomScrolledView.html">3.3.5.1 CustomScrolledView</a></a><br>
You can overried some XmRenderTable specific resources(XmNfontSize, XmNfontName) in .Xdefaults file in your application, and set the XmRenderTable
 to a graphics context for a pixmap to draw a text string in the pixmap.<br><br>

<a href="http://www.antillia.com/oz++/3.3.5.1_CustomScrolledView.html"><img src= "http://www.antillia.com/oz++/motif/images/CustomScrolledView.png" ></a>
 
<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.3.6"><a href="http://www.antillia.com/oz++/3.3.6_IsMotifASleepingBeauty.html">3.3.6 Is Motif a sleeping beauty?</a></a><br>
&nbsp; On Motif++, we have some experimental C++ classes(<font size=2 color="red">Xft++</font>) to use FontConfig and Xft on Motif.<br>
 This program ('Is Motif a sleeping beauty?') is an example to display UTF-8 strings by using FTFont (FreeTypeFont) and 
 FTFlatButton (FreeTypeFlatButton) classes of Xft++. <br>
</font>
<br>
<br>


<a href="http://www.antillia.com/oz++/3.3.6_IsMotifASleepingBeauty.html"><img src= "http://www.antillia.com/oz++/images/FTFlatButton.png"></a>
<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.3.7"><a href="http://www.antillia.com/oz++/3.3.7_FTFontPreview.html">3.3.7 FTFontPreview</a></a><br>
&nbsp;To see a list of known Fontconfig fonts, you can use <font size=2 color="red">fc-list</font> command in Fontconfig package.<br>
Using an output of fc-list, we can implement a font-preview C++ class to browse X11 Fontconfig fonts.<br>
 This is an example to display available fonts by FTFontPreview (FreeTypeFontPreview) of xft++.<br>
</font>
<br>
<a href="http://www.antillia.com/oz++/3.3.7_FTFontPreview.html"><img src= "http://www.antillia.com/oz++/images/FTFontPreview.png" ></a>
<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.3.8"><a href="http://www.antillia.com/oz++/3.3.8_FileListView.html">3.3.8_FileListView</a></a><br>
&nbsp;Open Motif 2.3 (<a href="http://www.google.co.jp/url?sa=t&rct=j&q=&esrc=s&source=web&cd=8&ved=0CFIQFjAH&url=http%3A%2F%2Ffossies.org%2Flinux%2Fmisc%2Fold%2Fopenmotif-2.3.0-manual.pdf.tgz%2Fappendixg.pdf&ei=HrvGVLSYAcG2mAXvyIHACw&usg=AFQjCNFUbt_IJKZoqVcp7xnRgOezSu-VLg&sig2=qZlEJtcl7lkz27cw6aC0RA&bvm=bv.84349003,d.dGY">Appendix G - New Features in Motif 2.3</a>) and later 
give programmers the ability to show pixmaps and label strings simultaneously in Labels or Buttons.<br>
 On OZ++, to create an instance of Label with a pixmap and a string label, you have to specify 
 <font size=2 color="red">XmPIXMAP_AND_STRING</font> for XmNlalbelType as shown below:<br>
</font>
<br>
<a href="http://www.antillia.com/oz++/3.3.8_FileListView.html"><img src= "http://www.antillia.com/oz++/images/FileListView.png" ></a>
<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.3.9"><a href="http://www.antillia.com/oz++/3.3.9_FileIconBox.html">3.3.9 FileIconBox</a></a><br>
&nbsp;Open Motif 2.3  and later provide some "Contributed Widgets" such as XmIconButton and XmIconBox classes,
which seem to be slightly similar to XmPushButton extension with a pixmap and XmButtonBox respectively. 
The XmIconButton can be used to show an icon (pixmap) and a label string simultaneously in a button.<br>
 On OZ++, to create an instance of IconButton, you have to specify 
 XmNlalbelString and XmNpixmap resources as shown below:<br>
<br>
</font>
<a href="http://www.antillia.com/oz++/3.3.9_FileIconBox.html"><img src= "http://www.antillia.com/oz++/images/ScrolledFileIconBox.png" ></a>
<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.3.10"><a href="http://www.antillia.com/oz++/3.3.10_IconBar.html">3.3.10 IconBar</a></a><br>
The XmIconButton in Open Motif 2.3 can also be used as a menubar by arranging the buttons to one line on a top region in ApplicationView .<br>

 This is an example to display a menu for some file editing operations by using IconBar class (motif++ 1.0.5). <br>
Please note that to add a button to the IconBar, we specify an icon filename to XmNiconFileName which is an OZ++ specific resource.<br>
<br>
</font>
<a href="http://www.antillia.com/oz++/3.3.10_IconBar.html"><img src= "http://www.antillia.com/oz++/images/IconBar.png" ></a>
<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.3.11"><a href="http://www.antillia.com/oz++/3.3.11_IPAddressWatcher.html">3.3.11 IPAddressWatcher</a></a><br>
&nbsp;On the latest Motif++ 1.0.6, we have introduced a very primitve Model-View update model on View class. 
By using the model, we can easily write a mixed program of an OZ++ Thread class and Motif++ GUI classes.<br>
&nbsp;Please note that the libraries of Open Motif 2.1 (and later) are <font size=2 color="red">thread-safe</font>(See: <a href="http://www.opengroup.org/openmotif/datasheet.html">Open Motif Overview</a>).<br>
&nbsp;This is an example to display some IPAddress information retrieved by IPAddressWatcher thread on 
TableView.<br>
 This sample program is built on two classes IPAddressWatcher(Thread), and MainView(ApplicationView).<br>
</font>
<br>
<a href="http://www.antillia.com/oz++/3.3.11_IPAddressWatcher.html"><img src= "http://www.antillia.com/oz++/images/IPAddressWatcher.png" ></a>
<br>
<br>

<font size=2>&nbsp;&nbsp;<a name="3.3.12"><a href="http://www.antillia.com/oz++/3.3.12_Polygon.html">3.3.12 Polygon</a></a><br>
&nbsp;On the latest Motif++ 1.0.7, we have implemented some experimental C++ classes (OpenGL++) for OpenGL.<br>
&nbsp;To display various OpenGL graphics objects on X11/Motif windows, we have introduced OpenGLView class based on Canvas class of the Motif++.<br>
&nbsp;This is an example to display a polygon on SimpleView class derived from OpenGLView.<br>
<br>
</font>
<a href="http://www.antillia.com/oz++/3.3.12_Polygon.html"><img src= "http://www.antillia.com/oz++/images/Polygon.png" ></a>
<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.3.13"><a href="http://www.antillia.com/oz++/3.3.13_ImageViewer.html">3.3.13 ImageViewer</a></a><br>
&nbsp;Although you may not be so interested in an image processing on X11/Motif, Open Motif 2.3 has added <font size=2 color="red">support of PNG and JPEG images</font> (<a href=<a href="http://www.google.co.jp/url?sa=t&rct=j&q=&esrc=s&source=web&cd=8&ved=0CFIQFjAH&url=http%3A%2F%2Ffossies.org%2Flinux%2Fmisc%2Fold%2Fopenmotif-2.3.0-manual.pdf.tgz%2Fappendixg.pdf&ei=HrvGVLSYAcG2mAXvyIHACw&usg=AFQjCNFUbt_IJKZoqVcp7xnRgOezSu-VLg&sig2=qZlEJtcl7lkz27cw6aC0RA&bvm=bv.84349003,d.dGY">Appendix G - New Features in Motif 2.3)</a>.
It will enable us to create easily better imaged widgets such as XmLabel and XmPushButton which have XmNlabelPixmap or XmNsomethingPixmap resources as shown below. <br>
<br>
</font>
<a href="http://www.antillia.com/oz++/3.3.13_ImageViewer.html"><img src= "http://www.antillia.com/oz++/images/ImageViewer.png" ></a>
<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.3.14"><a href="http://www.antillia.com/oz++/3.3.14_Texture.html">3.3.14 Texture</a></a><br>
&nbsp;The advanced image support of Motif is also helpful to create an OpenGL texture from an image file of PNG or JPEG. <br> 
&nbsp;This is an example to create an OpenGL texture from a pixmap loaded by using OZ::PixmapFile class
which is implemented based on XmGetPixmap API of Motif 2.3.<br>
<br>
</font>
<a href="http://www.antillia.com/oz++/3.3.14_Texture.html"><img src= "http://www.antillia.com/oz++/images/Texture.png" ></a>
<br>
<br>
<font size=2>&nbsp;&nbsp;<a href="http://www.antillia.com/oz++/3.3.15_InodeWatcher.html"><br><a name="3.3.15">3.3.15 InodeWatcher</a></a><br>
&nbsp;Linux provides some APIs (inotify_*) to monitor files(inodes). In OZ++ 2.0.15, we have added OZ::NonblockingInodeWatcher class to 
read inode change notifications from an inotify file descriptor of IN_NOBLOCK mode.<br>
&nbsp;This is an example to watch changes of inodes of Linux system by using OZ::NonblockingInodeWatcher class.<br>
<br>
</font>
<a href="http://www.antillia.com/oz++/3.3.15_InodeWatcher.html"><img src= "http://www.antillia.com/oz++/images/InodeWatcher.png" ></a>
<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.3.16"><a href="http://www.antillia.com/oz++/3.3.16_LightedTorus.html">3.3.16 LightedTorus</a></a><br>
&nbsp; We have been using <a href="http://freeglut.sourceforge.net/">FreeGLUT</a> which is 
a free-software/open-source alternative to the
 <a href="https://www.opengl.org/resources/libraries/glut/">OpenGL Utility Toolkit (GLUT) library</a>.
<br>
GLUT(FreeGULT) is a window system independent library, and very useful to develop simple and 
portable(cross-platform) OpenGL programs.<br>
&nbsp; This is an example to draw two toruses by using glutSolidTorus and glutWireTorus 
 APIs of GLUT library.
<br>

</font>
<br>
<a href="http://www.antillia.com/oz++/3.3.16_LightedTorus.html"><img src= "http://www.antillia.com/oz++/images/LightedTorus.png" ></a>
<br>
<br>

<font size=2>&nbsp;&nbsp;<a name="3.3.17"><a href="http://www.antillia.com/oz++/3.3.17_MultiJPGTexturedCubeRotationByTimerThread.html">3.3.17 MultiJPGTexturedCubeRotationByTimerThread</a></a><br>
&nbsp; We have C++ class OpenGLMultiTexturedCube to render a cube textured by multiple JPG or PNG image files.<br>
&nbsp;This is a simple sample program based on that class, and six JPG files for the cube fases.<br>
&nbsp;In this program the cube is rotated around y-axis by RenderingTimer Thead.
<br>
<br>
</font>
<a href="http://www.antillia.com/oz++/3.3.17_MultiJPGTexturedCubeRotationByTimerThread.html"><img src= "http://www.antillia.com/oz++/images/MultiJPGTexturedCubeRotationByTimerThread.png" ></a>
<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.3.18"><a href="http://www.antillia.com/oz++/3.3.18_MultiColoredTrianglShader.html">3.3.18 MultiColoredTrianglShader</a></a><br>
&nbsp; In OZ++2.0.23, we have updated our OpenGL C++ classes to use <a href="http://glew.sourceforge.net/">GLEW</a> in order to
create a context-version-dependent context, and to use the features of OpenGL Shader Language.
&nbsp; Currently, the default major version and the minor version for OpenGLContex class are 2 and 1 respectively.<br>  
&nbsp; This is a very simple sample program to use OpenGLProram, OpenGLFragmentShader, and OpenGLVertexShader
classes to draw a multicolored triangle. In this program, the sources for FragmentShader and VertexShader
are defined in the files of "fragmentShader.glsl" and "vertexShader.glsl" respectively.
<br>
<br>
</font>
<a href="http://www.antillia.com/oz++/3.3.18_MultiColoredTrianglShader.html"><img src= "http://www.antillia.com/oz++/images/MultiColoredTriangleShader.png" ></a>
<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.3.19"><a href="http://www.antillia.com/oz++/3.3.19_BufferedColoredRegularIcosahedron.html">3.3.19 BufferedColoredRegularIcosahedron</a></a><br>
&nbsp; In OZ++2.0.23, we have added new classes OpenGLBufferARB, OpenGLBufferedShapd, OpenGLIndexBufferARB, and OpenGLVertexBufferARB to 'oz++/openglarb/' folder to support GL_ARB_vertex_buffer_object extension in <a href="http://glew.sourceforge.net/">GLEW</a>.<br>
&nbsp; This is a very simple sample program to draw OpenGLColoredRegularIcosahedron by using those classes.
<br>
</font>
<br>
<a href="http://www.antillia.com/oz++/3.3.19_BufferedColoredRegularIcosahedron.html">  <img src= "http://www.antillia.com/oz++/images/BufferedColoredRegularIcosahedron.png" ></a>
    
<br>
<br>
<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.3.20"><a href="http://www.antillia.com/oz++/3.3.20_SphereWithColorEyeLightPositioner.html">3.3.20 SphereWithColorEyeLightPositioner</a></a><br>
&nbsp; In OZ++2.0.24 library, we have added new classes Positioner, ColorPositioner, EyePositioner, LightPositiner, and ColorEyeLightPositioner
 to 'oz++/motif' folder. Those have been implemented by Motif ScrollBar triplet.<br>
&nbsp;This is a very simple sample program to draw Sphere by using those classes. 
<br>
</font>
<br>
<a href="http://www.antillia.com/oz++/3.3.20_SphereWithColorEyeLightPositioner.html">  <img src= "http://www.antillia.com/oz++/images/SphereWithColorEyeLightPositioner.png" ></a>
<br>
<br>

<font size=2>&nbsp;&nbsp;<a name="3.3.21"><a href="http://www.antillia.com/oz++/3.3.21_MaterializedTorusesRotationByTimerThread.html">
3.3.21 MaterializedTorusesRotationByTimerThread</a></a><br>
&nbsp;  As you know, in OpenGL, there is a glMaterial API to set material properties.<br>
In the latest sol9.2.0, we have implemented <a href="http://www.antillia.com/oz++/classes/OpenGLMateria.html">OpenGLMateria</a> class to 
store material properites:face, ambient, diffuse, specular, emission, shininess.
<br>
<br>  
</font>
<br>
<a href="http://www.antillia.com/oz++/3.3.21_MaterializedTorusesRotationByTimerThread.html">  
<img src= "http://www.antillia.com/oz++/images/MaterializedTorusesRotationByTimerThread.png" ></a>


<br>
<br>

<font size=2>&nbsp;&nbsp;<a name="3.3.22"><a href="http://www.antillia.com/oz++/3.3.22_TexturedSphereRotationByKeyInput.html">3.3.22 TexturedSphereRotationByKeyInput</a></a><br>
&nbsp; In OpenGL, there are some methods to map a texture to a solid sphere.<br><br>
1. To use gluSphere and gluQuadricTexture APIs in OpenGL GLU library.
<br> See <a href="https://www.khronos.org/opengl/wiki/Texturing_a_Sphere">Texturing a Sphere</a>.
<br><br>
2. To write your own code to draw a sphere with texture coordinates.
<br>See <a href="http://www.codeincodeblock.com/2011/06/texture-map-in-solid-sphere-using.html">Texture map in solid sphere using GLUT(OpenGL) 
</a>.
<br>
<br>
Please note that you cannot use glutSolidSphere API of OpenGL GLUT library to map a texture on the sphere, 
because the API doesn't support texture coordinates.<br> 
 In the latest OZ++ library, we have implemented <a href="http://www.antillia.com/oz++/classes/OpenGLTexturedSphere.html">OpenGLTexturedSphere</a> class based on the first method.<br>
<br>  
The following TexturedSphereRotationByKeyInput is a simple example to draw a textured shpere and rotate it by Left or Rigth keys.<br>
<br>
In this example, we have used the world map of 'world.topo.bathy.200412.3x5400x2700.jpg' file in the following page.<br>
<a href="http://visibleearth.nasa.gov/view.php?id=73909">NASA VISIBLE EARTH</a><br>
December, Blue Marble Next Generation w/ Topography and Bathymetry<br>
Credit: Reto Stockli, NASA Earth Observatory<br>

</font>
<br>
<a href="http://www.antillia.com/oz++/3.3.22_TexturedSphereRotationByKeyInput.html">  
<img src= "http://www.antillia.com/oz++/images/TexturedSphereRotationByKeyInput.png" ></a>
<br>
<br>
<br>
<br>
<font size=2>
We have also used Viking's Mars 'mar0kuu2.jpg' created by Caltech/JPL/USGS in the <a href="https://maps.jpl.nasa.gov/mars.html">SOLAR SYSTEM SIMULATOR</a>.
</font>
<br>
<br>
<a href="http://www.antillia.com/oz++/3.3.22_TexturedSphereRotationByKeyInput.html">  <img src="http://www.antillia.com/oz++/images/TexturedSphereRotationByKeyInput_Mars.png" ></a>

<br>
<br>
<font size=2>
We have also used Jupiter 'PIA07782_hires.jpg' in the <a href="https://www.jpl.nasa.gov/spaceimages/details.php?id=PIA07782">JPL Cassini's Best Maps of Jupiter</a>.
</font>
<br>
<br>
<a href="http://www.antillia.com/oz++/3.3.22_TexturedSphereRotationByKeyInput.html">  <img src="http://www.antillia.com/oz++/images/TexturedSphereRotationByKeyInput_Jupiter.png" ></a>

<br>
<br>

<font size=2>&nbsp;&nbsp;<a name="3.3.23"><a href="http://www.antillia.com/oz++/3.3.23_StarSystemModel.html">3.3.23 StarSystemModel</a></a><br>
In OpenGL, we can draw a simple star system model like our solar system.  
We assume an imaginary star system which consists of a single sun-like sphere star 
and some planet-like spheres rotating on circular oribits around the sun star
, and furthermore, to avoid confusion, we do not care the light direction and reflection light problem of sun and the planets.
 
<br>  
</font>
<br>
<a href="http://www.antillia.com/oz++/3.3.23_StarSystemModel.html">  
<img src= "http://www.antillia.com/oz++/images/StarSystemModel.png" ></a>

<!--  -->
<br>
<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.3.24"><a href="http://www.antillia.com/oz++/3.3.24_FileSystemBrowser.html">3.3.24 FileSystemBrowser</a></a><br>
<br>  
</font>
<font size=2>
 As you know, Motif has an xmFileSelectionBoxWidgetClass to select a file as shown below:<br><br>
<img src= "http://www.antillia.com/oz++/images/FileSelectionDialog.png" > 
<br><br>
 We have implemented <a href="http://www.antillia.com/oz++/classes/FileBox.html">FileBox</a> and <a href="http://www.antillia.com/oz++/classes/FileDialog.html">
 FileDialog</a> to use the widget class.
  
 Unfortunately, this is not so convenient to browse folders and select files on
  a whole Linux file system. <br> <br>
 In the latest OZ++ class library, we have implemented a very simple experimental class
 <a href="http://www.antillia.com/oz++/classes/FileSystemBrowser.html">FileSystemBrowser</a> as a subclass of PopupView,
  by using the following two components:<br><br>
<a href="http://www.antillia.com/oz++/classes/FolderSelectionBox.html">FolderSelectionBox</a> which is a subclass of ScrolledWindow to 
wrap xmContainerWidgetClass and xmIconGadgetClass of Motif.
<br>
<br>
<a href="http://www.antillia.com/oz++/classes/FileListView.html">FileListView</a> which is a sublcas of ScrolledWindow to display folders or fileslist.
<br>  
</font>
<br>
<a href="http://www.antillia.com/oz++/3.3.24_FileSystemBrowser.html">  
<img src= "http://www.antillia.com/oz++/images/FileSystemBrowser.png" ></a>

<br>
<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.3.25"><a href="http://www.antillia.com/oz++/3.3.25_MedianBlurView.html">3.3.25 ImageBlur</a></a><br>
<br>  
</font>
<font size=2>
 As you know, OpenCV has some image filter classes and image-blur APIs <a href="http://docs.opencv.org/2.4/doc/tutorials/imgproc/gausian_median_blur_bilateral_filter/gausian_median_blur_bilateral_filter.html">
Smoothing Images</a> such as:<br>
<br>
blur<br>
GaussianBlur<br>
medianBlur<br>
bilateralFilter<br>
<br>
 The following MedianBlur program is a simple example to blur an image based on mediaBlur funcition. <br>
The left pane is an orignal image, and the right pane is a blurred image created by a KernelSize trackbar control.
<br>  

</font>
<br>
<a href="http://www.antillia.com/oz++/3.3.25_MedianBlurView.html">  
<img src= "http://www.antillia.com/oz++/images/MedianBlurView.png" ></a>

<br>
<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.3.26"><a href="http://www.antillia.com/oz++/3.3.26_ImageSharpening.html">3.3.26 ImageSharpen</a></a><br>
<br>  
</font>
<font size=2>
 As shown in <a href="https://en.wikipedia.org/wiki/Unsharp_masking#Digital_unsharp_masking">Unsharp masking</a>,
 Unsharp masking (USM) is one of the image sharpening techniques. It uses a smoothed (blurred), or "unsharp", negative image 
 to create a mask of the original image.<br>
The following imageSharpening program is a simple image sharpening example based on the USM technique, in which we have used the following two APIs: <br>
<br> 
1 GaussinaBlur to create a blurred mask image from an original image.<br>
<br>
2 addWeighted to combine the blurred image and the original image. <br>
<br>
The left pane is an orignal image, and the right pane is a sharpened image created by KernelSize and Sigma trackbar controls.

</font>
<br>
<a href="http://www.antillia.com/oz++/3.3.26_ImageSharpening.html">  
<img src= "http://www.antillia.com/oz++/images/ImageSharpening.png" ></a>
<br>
<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.3.27"><a href="http://www.antillia.com/oz++/3.3.27_ObjectDetector.html">3.3.27 ObjectDetector</a></a><br>
<br>  
</font>
<font size=2>
 One of the most interesting feature of OpenCV is an image recognition or object detection, which is a very important technology
 in modern computer vision based systems.<br>
 For detail, please see the OpenCV documentation <a href="http://docs.opencv.org/2.4/modules/objdetect/doc/cascade_classification.html">
Cascade Classification Haar Feature-based Cascade Classifier for Object Detection</a>.<br><br>

 In the latest OpenCV version, the following two types of cascade classifiers are available:<br><br>
 <li>Harr Feature-based Cascade Classifier</li><br>
 <li>LBP(Local binary pattern) Cascade Classifier</li><br>
 
On these cascade classifiers, please check your OpenCV installed directory, for example, you can find folders including classifier files
 under the folder "opencv/data/.
<br> 
 The following ObjectDetector is a simple object detection example. <br><br>
1 Select a folder which  contains Harr or LBP cascade classifiers by using a folder open dialog which can be popped up by clicking [...] button<br>
2 Select a cascade classifier xml file in a combobox.<br>
3 Open a target image file by using a file open dialog which can be popped up by Open menu item.<br>
4 Click Detect pushbutton.<br>
</font>
<br>

<br>
<a href="http://www.antillia.com/oz++/3.3.27_ObjectDetector.html">  
<img src= "http://www.antillia.com/oz++/images/ObjectDetector1.png" ></a>
<br>
<br>
<a href="http://www.antillia.com/oz++/3.3.27_ObjectDetector.html">  
<img src= "http://www.antillia.com/oz++/images/ObjectDetector2.png" ></a>
<br>
<br>
<a href="http://www.antillia.com/oz++/3.3.27_ObjectDetector.html">  
<img src= "http://www.antillia.com/oz++/images/ObjectDetector3.png" ></a>
<br>
<!-- 2017/09/10 -->
<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.3.28"><a href="http://www.antillia.com/oz++/3.3.28_VideoDeviceEnumerator.html">3.3.28 VideoDeviceEnumerator</a></a><br>
<br>  
</font>
<font size=2>
 You can read a video image from a video input device somthing like a web-camera by using 
 
 <a href="http://docs.opencv.org/3.3.0/d8/dfe/classcv_1_1VideoCapture.html">cv::VideoCapture</a> class of OpenCV. <br>
In Linux enviroment, you have to install <a href="https://launchpad.net/ubuntu/precise/amd64/libv4l-dev">libv4l2-dev</a> package.<br>
After the installation, you can see video devices on the folder /sys/class/video4linux/ as shown below.<br>
<br>
<img src = "http://www.antillia.com/oz++/images/video4linux.png">
<br>
<br>
In this case, you have two video devices corresponding to the folders video0 and video1.

By enumerating all device folders under the main folder /sys/class/video4linux, you can get all device informaton, and create a combobox to listup them 
and select a device index by a device name.<br><br>

In the latest OZ++ library, we have implemented VideoDevice, VideoInputDeviceEnumerator, and LabeledVideoCombobox classes
to listup all video devices and select a device.
<br>
<br>
The following VideoDeviceEnumerator program is a simple image capturing example based on the LabeledVideoDeviceComboBox, and VideoCapture classes 
of OZ++.
<br>
<br>
<a href="http://www.antillia.com/oz++/3.3.28_VideoDeviceEnumerator.html">  
<img src= "http://www.antillia.com/oz++/images/VideoDevice.png" ></a>
<br><br>
<a href="http://www.antillia.com/oz++/3.3.28_VideoDeviceEnumerator.html">  
<img src= "http://www.antillia.com/oz++/images/VideoDeviceEnumerator.png" ></a>
<br>

</font>
<br>
<br>

<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.3.29"><a href="http://www.antillia.com/oz++/3.3.29_OpenGLCVImageViews.html">3.3.29 OpenGLCVImageViews</a></a><br>
<br>  
</font>
<font size=2>
 Imagine to map a cv::Mat image of OpenCV to a shape of OpenGL as a texture by using an image method of
 <a href="http://www.antillia.com/oz++/classes/OpenGLTexture2D.html">OpenGLTexture2D</a> class of OZ++. <br>
In the latest OZ++ library, we have implemented OpenGLImageInfo and OpenCVImageInfo classes to extract the raw image data from a cv::Mat.<br>
We have also implemented <a href="http://www.antillia.com/oz++/classes/OpenGLBulletinBoard.html">OpenGLBulletinBoard</a> class to layout an OpenCVImageView and an OpenGLView simultaneously on one bulletinboard. <br>

<br>
The following OpenGLCVImageViews program is a simple example to display an OpenCVImageView and an OpenGLView side by side. In this example, we get an OpenGLImageInfo from
the cv::Mat image displayed on the left pane by using OpenCVImageInfo class, and map the OpenGLImageInfo to a quad shape of OpenGL of the right pane as a texture.<br>
  
of OZ++.
<br>
<a href="http://www.antillia.com/oz++/3.3.29_OpenGLCVImageViews.html">  
<img src= "http://www.antillia.com/oz++/images/OpenGLCVImageViews.png" ></a>
<br>

<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.3.30"><a href="http://www.antillia.com/oz++/3.3.30_BlurredMaterializedSpheres.html">3.3.30 BlurredMaterializedSpheres</a></a><br>
<br>  
</font>
<font size=2>
 Imagine to read pixel data of an OpenGL frame buffer and convert it to a cv::Mat image format of OpenCV.<br>
Reading pixel data of the frame buffer can be done by using glReadPixels API of OpenGL, and creating a cv::Mat image 
by calling its constructor in the following way in a subclass, say SimpleGLView, derived from OpenGLView :<br>
<br>
class SimpleGLView : public OpenGLView {<br><br> 
&nbsp;&nbsp;cv::Mat&nbsp;capture()<br>
&nbsp;&nbsp;{<br>
&nbsp;&nbsp;&nbsp;&nbsp;Dimension&nbsp;w&nbsp;=&nbsp;width();<br>
&nbsp;&nbsp;&nbsp;&nbsp;Dimension&nbsp;h&nbsp;=&nbsp;height();<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;w&nbsp;=&nbsp;(w/8)*8;<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;glReadBuffer(GL_FRONT);<br>
&nbsp;&nbsp;&nbsp;&nbsp;glPixelStorei(GL_UNPACK_ALIGNMENT,&nbsp;1);<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;unsigned&nbsp;char*&nbsp;pixels&nbsp;=&nbsp;new&nbsp;unsigned&nbsp;char[3&nbsp;*&nbsp;w&nbsp;*&nbsp;h];<br>
&nbsp;&nbsp;&nbsp;&nbsp;glReadPixels(0,&nbsp;0,&nbsp;w,&nbsp;h,&nbsp;GL_BGR,&nbsp;GL_UNSIGNED_BYTE,&nbsp;pixels);<br>
&nbsp;&nbsp;&nbsp;&nbsp;<br>
&nbsp;&nbsp;&nbsp;&nbsp;return&nbsp;cv::Mat(h,&nbsp;w,&nbsp;CV_8UC3,&nbsp;pixels);<br>
&nbsp;&nbsp;}<br>
};<br>
<br>  

The following BlurredMaterializedSpheres program is a simple example to display some materialized shperes on an OpenGLView and a blurred cv::Mat image 
,corresponding to the frame buffer of the OpenGLView, on an OpenCVImageView side by side. 

<br>
<br>
<a href="http://www.antillia.com/oz++/3.3.30_BlurredMaterializedSpheres.html">  
<img src= "http://www.antillia.com/oz++/images/BlurredMaterializedSpheres.png" ></a>
<br>
</font>
<br>
<br>

<br>
<font size=2>&nbsp;&nbsp;<a name="3.3.31"><a href="http://www.antillia.com/oz++/3.3.31_ImageTransformationByDynamicColorFilter.html">3.3.31 ImageTransformationByDynamicColorFilter</a></a><br>
<br>  
</font>
<font size=2>
As you know, you can transform an image by using a cv::Mat filter(Kernel) and cv::transform API of OpenCV
as shown below.<br>
</font>
<br>

<font size=2>
&nbsp;&nbsp;&nbsp;&nbsp;void applySepiaFilter(const char* filename)<br>
&nbsp;&nbsp;&nbsp;&nbsp;{<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cv::Mat originalImage = cv::imread(filename, cv::IMREAD_COLOR);<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cv::Mat transformedImage = originalImage.clone();<br>
<br>    
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;//sepia filter<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cv::Mat sepia = (cv::Mat_&lt;float&gt;(3,3) &lt;&lt; <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.272, 0.534, 0.131,<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.349, 0.686, 0.168, <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0.393, 0.769, 0.189); <br>
<bf>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cv::transform(originalImage, sepia, transformedImage);<br>
&nbsp;&nbsp;&nbsp;&nbsp;  }<br>
</font>

<br>
<font>
In this case, the originalImage is transformed by the constant sepia filter to the transformedImage.
This is a traditional constant filter example, however, it is much better to be able to
 apply a dynamically changeable color filter (Kernel) to a cv::Mat image.<br>
<br>
The following ImageTransformationByDynamicColorFilter is a simple example to implement a dynamic color filter
by using OpenCVColorFilter class and ColorPositioner class of SOL9.<br>
<br>
</font>

<br>
<a href="http://www.antillia.com/oz++/3.3.31_ImageTransformationByDynamicColorFilter.html">  
<img src= "http://www.antillia.com/oz++/images/ImageTransformationByDynamicColorFilter.png" ></a>
<br>
</font>
<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.3.32"><a href="http://www.antillia.com/oz++/3.3.32_TexturedSphereWithAxisEyeAndLightPositioner.html">3.3.32 TexturedSphereWithAxisEyeAndLightPositioner</a></a><br>
<br>  
</font>
 As show in <a href="http://www.antillia.com/oz++/3.3.22_TexturedSphereRotationByKeyInput.html">3.3.22_TexturedSphereRotationByKeyInput</a>, it is easy to render and rotate a sphere textured by an image file in OpenGL.<br>
If we could dynamically change a rotation axis, an eye position, and a light position in that example, however, it would be a much better program.<br><br> 

The following TexturedSphereWithAxisEyeAndLightPositioner is a simple example to render a sphere textured by a cylindrical planet map.<br>

In this example, we have used Venus map 'ven0aaa2.jpg' Caltech/JPL/USGS in the page
<a href="https://maps.jpl.nasa.gov/venus.html">JPL NASA SOLAR SYSTEM SIMULATOR </a>.<br>
<br>

<a href="http://www.antillia.com/oz++/3.3.32_TexturedSphereWithAxisEyeAndLightPositioner.html"><img src="http://www.antillia.com/oz++/images/TexturedSphereWithAxisEyeAndLightPositioner_Venus.png"></a>
<br><br>
<font size=2>
We have also used Lunar Orbiter's 'Lunar_LO_UVVISv2_Hybrid_Mosaic_Global_1024.jpg' in the page
<a href="https://astropedia.astrogeology.usgs.gov/download/Moon/Lunar-Orbiter/thumbs/Lunar_LO_UVVISv2_Hybrid_Mosaic_Global_1024.jpg">Astrogeology Science Center</a>.
</font>
<br>
<br>
<a href="http://www.antillia.com/oz++/3.3.32_TexturedSphereWithAxisEyeAndLightPositioner.html">  <img src="http://www.antillia.com/oz++/images/TexturedSphereWithAxisEyeAndLightPositioner_Moon.png" ></a>

<br>
<br>
Furthermore, we have used Voyager Galileo IO 'jup1vss2.jpg' in the page 
<a href="https://maps.jpl.nasa.gov/jupiter.html">JPL NASA SOLAR SYSTEM SIMULATOR </a>
<br>
<br>
<a href="http://www.antillia.com/oz++/3.3.32_TexturedSphereWithAxisEyeAndLightPositioner.html">  <img src="http://www.antillia.com/oz++/images/TexturedSphereWithAxisEyeAndLightPositioner_IO.png" ></a>

<!-- 2019/06/12 -->
<br>
<br>
<font size=2>&nbsp;&nbsp;<a name="3.3.33"><a href="http://www.antillia.com/oz++/3.3.33_CustomYoloObjectDetector.html">3.3.33 CustomYoloObjectDetector</a></a><br>
<br>  
</font>
As you may know, the latest YOLOv3 contains a C++ Detetor class implementation (darknet-master/include/Detector.h).
You can also define your own Detector C++ class based on C APIs of detector.c, image.c, etc. in darknet-master/src folders.

The following CustomYoloObjectDetector is a simple example to use such a Detector C++ class.<br>

<br>
<a href="http://www.antillia.com/oz++/3.3.33_CustomYoloObjectDetector.html"><img src="http://www.antillia.com/oz++/yolov3/images/CustomYoloObjectDetector.png"></a>
<br>
<br>

<a href="http://www.antillia.com/oz++/3.3.33_CustomYoloObjectDetector.html"> <img src="http://www.antillia.com/oz++/yolov3/images/CustomYoloObjectDetector.2.png" ></a>
<br>
<br>

<a href="http://www.antillia.com/oz++/3.3.33_CustomYoloObjectDetector.html"> <img src="http://www.antillia.com/oz++/yolov3/images/CustomYoloObjectDetector.3.png" ></a>
<br>
<br>

<!-- bottom anchors end -->


<!-- Right End -->

<!--
================================================================================================
-->

</td>

</tr>
</table>

<font size = 2 color = "navy" >
<hr noshade color="navy">
&nbsp;<b>Last modified: 12 Feb. 2020</b>
</font>
<br>
<font color = "navy" size ="2">
<b>
Copyright (c) 2000-2020  Antillia.com ALL RIGHTS RESERVED. 
</b>
</font>
</body>
</html>


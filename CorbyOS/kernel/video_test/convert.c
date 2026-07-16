/*
    CODE BY ArTicZera 07/??/2026
*/

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdint.h>

typedef unsigned char BYTE;

#define WIDTH  160
#define HEIGHT 120

#define FRAME_SIZE (WIDTH * HEIGHT / 8)

int main()
{
    cv::VideoCapture cap("video.mkv");

    if (!cap.isOpened())
    {
        return 1;
    }

    int total_frames = (int)cap.get(cv::CAP_PROP_FRAME_COUNT);
    double fps = 15;

    FILE* out = fopen("video.bin", "wb");

    if (!out)
    { 
        return 1;
    }

    cv::Mat frame, gray, bw;
    int frame_count = 0;

    while (true)
    {
        if (!cap.read(frame))
        {
            break;
        }

        frame_count++;

        if (frame.empty())
        {
            continue;
        }

        cv::resize(frame, frame, cv::Size(WIDTH, HEIGHT), 0, 0, cv::INTER_AREA);
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::threshold(gray, bw, 127, 255, cv::THRESH_BINARY);

        BYTE buffer[FRAME_SIZE];
        int index = 0;

        for (int y = 0; y < HEIGHT; y++)
        {
            for (int x = 0; x < WIDTH; x += 8)
            {
                BYTE byte = 0;

                for (int b = 0; b < 8; b++)
                {
                    BYTE pixel = bw.at<BYTE>(y, x + b);

                    if (pixel) byte |= (1 << (7 - b));
                }

                buffer[index++] = byte;
            }
        }

        if (index != FRAME_SIZE)
        {
            break;
        }

        fwrite(buffer, 1, FRAME_SIZE, out);
    }

    fclose(out);
    cap.release();

    printf("Finalizado! Frames processados: %d\n", frame_count);

    return 0;
}
struct BasicFOCController {
    struct Command {
        float iq, id, vq, vd;
    };
    void update(const Command &command) { kp++; }
    int kp = 2;
};

#include "args.h"
#include <unordered_map>

// This approach is lovingly borrowed from: https://blog.vito.nyc/posts/min-guide-to-cli/
args::Args ParseArgsArgMap(int argc, char** argv) {
    typedef std::function<void(args::Args&, const std::string&)> OneArgHandle;
    typedef std::function<void()> NoArgHandle;

    const std::unordered_map<std::string, OneArgHandle> SINGLE_ARG_MAP{
        {"--render", [](args::Args& args, const std::string& str_arg) { args.render_option_ = str_arg; }},
        {"-r", [](args::Args& args, const std::string& str_arg) { args.render_option_ = str_arg; }},
        {"--image-width",
         [](args::Args& args, const std::string& str_arg) { args.image_width_ = std::stoi(str_arg); }},
        {"-i", [](args::Args& args, const std::string& str_arg) { args.image_width_ = std::stoi(str_arg); }}};

    const std::unordered_map<std::string, NoArgHandle> NO_ARG_MAP{
        {"--help", []() { args::ShowHelp(); }},
        {"-h", []() { args::ShowHelp(); }},
    };

    // retval
    args::Args args{};
    // mangled/missing args
    bool should_show_help{false};

    // avoid executable name (argv[0])
    for (int i = 1; i < argc; i++) {
        const std::string opt{argv[i]};

        if (auto no_arg_iterator{NO_ARG_MAP.find(opt)}; no_arg_iterator != NO_ARG_MAP.end()) {
            // only no arg is "help"; ignore all remaining args
            no_arg_iterator->second();
            break;
        }

        if (const auto single_arg_iterator{SINGLE_ARG_MAP.find(opt)};
            single_arg_iterator != SINGLE_ARG_MAP.end()) {
            if (++i < argc) {
                // handle arg
                single_arg_iterator->second(args, {argv[i]});
            } else {
                should_show_help = true;
                break;
            }
        }
    }

    if (should_show_help) {
        args::ShowHelp();
    }
    return args;
}

// TODO: investigate this is strangely broken (segfaults on release builds(?))
[[maybe_unused]] args::Args ParseArgsGetOptLong(int argc, char** argv) {
    const char* const short_opts = "r:i:";
    const option long_opts[] = {{"render", required_argument, nullptr, 'r'},
                                {"image-width", required_argument, nullptr, 'i'}};

    std::optional<std::string> render_option{};
    std::optional<u_int16_t> image_width{};

    while (true) {
        const auto option = getopt_long(argc, argv, short_opts, long_opts, nullptr);
        if (option == -1) {
            break;
        }

        switch (option) {
            case 'r': {
                render_option = std::string(optarg);
                break;
            }
            case 'i': {
                // this makes no sense why this is "required"
                // I'm clearly not using this API correctly.
                if (optarg) {
                    image_width = std::stoi(optarg);
                }
                break;
            }
            case 'h':
            default:
                args::ShowHelp();
                break;
        }
    }
    return {render_option, image_width};
}

args::Args args::ParseArgs(int argc, char** argv) {
    return ParseArgsArgMap(argc, argv);
}

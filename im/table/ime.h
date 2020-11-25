/*
 * SPDX-FileCopyrightText: 2017-2017 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 */
#ifndef _TABLE_TABLEDICTRESOLVER_H_
#define _TABLE_TABLEDICTRESOLVER_H_

#include <fcitx-config/configuration.h>
#include <fcitx-config/enum.h>
#include <fcitx-utils/i18n.h>
#include <fcitx-utils/log.h>
#include <fcitx/candidatelist.h>
#include <libime/core/userlanguagemodel.h>
#include <libime/table/tablebaseddictionary.h>
#include <tuple>

namespace fcitx {

FCITX_CONFIG_ENUM(OrderPolicy, No, Freq, Fast);
FCITX_CONFIG_ENUM_NAME(CandidateLayoutHint, "Not set", "Vertical",
                       "Horizontal");
FCITX_CONFIG_ENUM_I18N_ANNOTATION(CandidateLayoutHint, N_("Not set"),
                                  N_("Vertical"), N_("Horizontal"));

FCITX_CONFIGURATION(
    TableConfig, HiddenOption<std::string> file{this, "File", _("File")};
    KeyListOption prevPage{
        this,
        "PrevPage",
        _("Prev page"),
        {Key(FcitxKey_Up)},
        KeyListConstrain(KeyConstrainFlag::AllowModifierLess)};
    KeyListOption nextPage{
        this,
        "NextPage",
        _("Next page"),
        {Key(FcitxKey_Down)},
        KeyListConstrain(KeyConstrainFlag::AllowModifierLess)};
    KeyListOption prevCandidate{
        this,
        "PrevCandidate",
        _("Prev Candidate"),
        {Key("Left")},
        KeyListConstrain(KeyConstrainFlag::AllowModifierLess)};
    KeyListOption nextCandidate{
        this,
        "NextCandidate",
        _("Next Candidate"),
        {Key("Right")},
        KeyListConstrain(KeyConstrainFlag::AllowModifierLess)};
    KeyListOption secondCandidate{
        this,
        "SecondCandidate",
        _("Select Second Candidate"),
        {},
        KeyListConstrain({KeyConstrainFlag::AllowModifierLess,
                          KeyConstrainFlag::AllowModifierOnly})};
    KeyListOption thirdCandidate{
        this,
        "ThirdCandidate",
        _("Select Third Candidate"),
        {},
        KeyListConstrain({KeyConstrainFlag::AllowModifierLess,
                          KeyConstrainFlag::AllowModifierOnly})};
    HiddenOption<KeyList> selection{
        this,
        "Selection",
        _("Selection"),
        {Key(FcitxKey_1), Key(FcitxKey_2), Key(FcitxKey_3), Key(FcitxKey_4),
         Key(FcitxKey_5), Key(FcitxKey_6), Key(FcitxKey_7), Key(FcitxKey_8),
         Key(FcitxKey_9), Key(FcitxKey_0)}};
    Option<int, IntConstrain> pageSize{this, "PageSize", _("Page size"), 5,
                                       IntConstrain(3, 10)};
    // Fcitx 4 default behavior.
    Option<bool> commitAfterSelect{this, "CommitAfterSelect",
                                   _("Commit after auto select candidates"),
                                   true};
    Option<bool> useFullWidth{this, "UseFullWidth", _("Use full width"), true};
    Option<bool> ignorePunc{this, "IgnorePunc",
                            _("Ignore built in punctuation"), false};
    Option<bool> firstCandidateAsPreedit{this, "FirstCandidateAsPreedit",
                                         _("First candidate as Preedit"),
                                         false};
    Option<Key, KeyConstrain> quickphrase{
        this,
        "QuickPhraseKey",
        _("Key to trigger quickphrase"),
        Key{},
        {KeyConstrainFlag::AllowModifierLess}};
    HiddenOption<std::string> icon{this, "Icon", _("Icon")};
    Option<int> noSortInputLength{this, "NoSortInputLength",
                                  _("Don't sort word shorter than")};
    Option<OrderPolicy> orderPolicy{this, "OrderPolicy", _("Order policy")};
    OptionWithAnnotation<bool, ToolTipAnnotation> useSystemLanguageModel{
        this,
        "UseSystemLanguageModel",
        _("Use system language model"),
        true,
        {},
        {},
        ToolTipAnnotation(_("When available, system language model will be "
                            "loaded for given language and provide ability to "
                            "give better context based order."))};
    OptionWithAnnotation<bool, ToolTipAnnotation> useContextBasedOrder{
        this,
        "UseContextRelatedOrder",
        _("Sort candidates based on current context"),
        true,
        {},
        {},
        ToolTipAnnotation(
            _("Order the candidate based on the word you already typed. You "
              "may want to disable this option if you want the candidate order "
              "to be stable regardless of the history."))};
    Option<Key, KeyConstrain> matchingKey{
        this,
        "MatchingKey",
        _("Wildcard matching Key"),
        Key(),
        {KeyConstrainFlag::AllowModifierLess}};
    Option<Key, KeyConstrain> pinyinKey{this,
                                        "PinyinKey",
                                        _("Prefix key to trigger Pinyin"),
                                        Key(),
                                        {KeyConstrainFlag::AllowModifierLess}};
    Option<bool> autoSelect{this, "AutoSelect", _("Auto select candidate")};
    OptionWithAnnotation<int, ToolTipAnnotation> autoSelectLength{
        this,
        "AutoSelectLength",
        _("Length limit of selecting the only candidate"),
        0,
        {},
        {},
        ToolTipAnnotation(
            _("When current input length is greater than or equal to the limit "
              "and there is only one candidate available, automatically select "
              "this candidate. -1 Means the maximum code length of the table. "
              "0 means this behavior is disabled."))};
    OptionWithAnnotation<bool, ToolTipAnnotation> commitInvalidSegment{
        this,
        "CommitInvalidSegment",
        _("Commit Invalid Segment"),
        false,
        {},
        {},
        ToolTipAnnotation(
            _("Commit the segment that does not exist in the table. This "
              "option is useful for some latin/compose based table."))};
    OptionWithAnnotation<int, ToolTipAnnotation> noMatchAutoSelectLength{
        this,
        "NoMatchAutoSelectLength",
        _("Auto select if current input has no match"),
        0,
        {},
        {},
        ToolTipAnnotation(
            _("When typing a new character, if the current input length is "
              "greater than or equal to the limit, and the existing segment "
              "with the new character does not have any matched entries in the "
              "table, select the current candidate and then type in the new "
              "character. -1 means the maximum code length of the table. 0 "
              "means this behavior is disabled."))};
    HiddenOption<KeyList> endKey{this, "EndKey", _("End key")};
    OptionWithAnnotation<int, ToolTipAnnotation> autoPhraseLength{
        this,
        "AutoPhraseLength",
        _("Auto phrase length"),
        -1,
        {},
        {},
        ToolTipAnnotation(
            _("After committing every character, learn the new phrase based on "
              "the input history. If certain word is seen for multiple times "
              "or selected by user once, it will be added as user phrase. -1 "
              "means the maximum code length of the table. 0 means disable "
              "this feature. "))};
    OptionWithAnnotation<int, ToolTipAnnotation> saveAutoPhraseAfter{
        this,
        "SaveAutoPhraseAfter",
        _("Save auto phrase after being typed for ... times"),
        -1,
        {},
        {},
        ToolTipAnnotation(
            _("If the value less than 0, means this feature is disabled. If "
              "auto phrase is enabled, selecting auto phrase candidate "
              "explicitly will still add the word as a new user phrase."))};
    Option<bool> exactMatch{this, "ExactMatch", _("Exact Match")};
    Option<bool> learning{this, "Learning", _("Learning"), true};
    Option<bool> hint{this, "Hint", _("Display Hint for word")};
    Option<bool> displayCustomHint{this, "DisplayCustomHint",
                                   _("Display custom hint")};
    OptionWithAnnotation<CandidateLayoutHint, CandidateLayoutHintI18NAnnotation>
        candidateLayoutHint{this, "CandidateLayoutHint",
                            _("Candidate List orientation"),
                            CandidateLayoutHint::NotSet};
    HiddenOption<std::vector<std::string>> autoRuleSet{this, "AutoRuleSet",
                                                       _("Auto rule set")};);

FCITX_CONFIGURATION(PartialIMInfo, HiddenOption<std::string> languageCode{
                                       this, "LangCode", "Language Code"};);

struct NoSaveAnnotation {
    bool skipDescription() const { return true; }
    bool skipSave() const { return true; }
    void dumpDescription(RawConfig &) const {}
};

FCITX_CONFIGURATION(TableConfigRoot,
                    Option<TableConfig> config{this, "Table", "Table"};
                    Option<PartialIMInfo, NoConstrain<PartialIMInfo>,
                           DefaultMarshaller<PartialIMInfo>, NoSaveAnnotation>
                        im{this, "InputMethod", "InputMethod"};);

struct TableData {
    TableConfigRoot root;
    std::unique_ptr<libime::TableBasedDictionary> dict;
    std::unique_ptr<libime::UserLanguageModel> model;
};

class TableIME {
public:
    TableIME(libime::LanguageModelResolver *lmResolver);

    const TableConfig &config(const std::string &name);

    std::tuple<libime::TableBasedDictionary *, libime::UserLanguageModel *,
               const TableConfig *>
    requestDict(const std::string &name);
    void saveDict(const std::string &name);
    void saveAll();
    void updateConfig(const std::string &name, const RawConfig &config);

    void releaseUnusedDict(const std::unordered_set<std::string> &names);

private:
    libime::LanguageModelResolver *lm_;
    std::unordered_map<std::string, TableData> tables_;
};

FCITX_DECLARE_LOG_CATEGORY(table_logcategory);

#define TABLE_DEBUG() FCITX_LOGC(::fcitx::table_logcategory, Debug)
#define TABLE_ERROR() FCITX_LOGC(::fcitx::table_logcategory, Error)
} // namespace fcitx

#endif // _TABLE_TABLEDICTRESOLVER_H_

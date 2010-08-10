require File.expand_path('../../spec_helper', __FILE__)

describe "Date constants" do

  it "should define MONTHNAMES" do
    Date::MONTHNAMES.should == [nil] + %w(January February March April May June July
                                          August September October November December)
  end
  
  it "should define DAYNAMES" do
    Date::DAYNAMES.should == %w(Sunday Monday Tuesday Wednesday Thursday Friday Saturday)
  end
  
  it "should define ABBR_MONTHNAMES" do
    Date::ABBR_DAYNAMES.should == %w(Sun Mon Tue Wed Thu Fri Sat)
  end

end

require File.expand_path('../../spec_helper', __FILE__)

describe "Date.allocate" do
  it "should be the same as jd" do
    Date.allocate.should == Date.jd
  end
end
